#include "OVRRenderer-deepoon.h"

USING_NS_CC;

class OVRHelper
{
public:
	OVRHelper();
	~OVRHelper();

	static JavaVM *javaVM;
	static jobject activity;
};

JavaVM * OVRHelper::javaVM = nullptr;
jobject OVRHelper::activity = 0;

void cocos_android_app_onCreate(JNIEnv* env, jobject activity) {
	CCLOG("cocos_android_app_onCreate Begin");
	env->GetJavaVM(&OVRHelper::javaVM);
	//OVRHelper::java.Vm->AttachCurrentThread(&OVRHelper::java.Env, NULL);
	OVRHelper::activity = env->NewGlobalRef(activity);
	CCLOG("cocos_android_app_onCreate End");
}

//////////////////////////////////////////////////////////////////////////////////////////////

#define NUM_MULTI_SAMPLES	4
#define RENDER_ORDER_ID 100.0f

const int EYE_BUFFER_SIZE = 1024;

OVRRenderer::OVRRenderer()
	: _instance(nullptr)
{
	for (int eye = 0; eye < EYE_NUM; eye++)
	{
		_eyeCamera[eye] = nullptr;
	}
}

OVRRenderer::~OVRRenderer()
{
	//vrapi_Shutdown();
}

bool OVRRenderer::init(cocos2d::CameraFlag flag)
{
	CCLOG("OVRRenderer::init");

	for (int eye = 0; eye < EYE_NUM; eye++)
	{
		dpnnFramebuffer_Clear(&_frameBuffer[eye]);
		dpnnFramebuffer_Create(&_frameBuffer[eye],
			EYE_BUFFER_SIZE,
			EYE_BUFFER_SIZE,
			NUM_MULTI_SAMPLES);
	}

	auto backToForegroundListener = EventListenerCustom::create(EVENT_COME_TO_FOREGROUND,
		[=](EventCustom*)
	{
		CCLOG("OVRRenderer::vrapi_EnterVrMode");

		_instance = dpnnInit(1, DPNN_UM_DEFAULT, NULL, DPNN_DEVICE_GLES2, (void*)OVRHelper::activity);
		CCLOG("OVRHelper::dpInstance 0x%x", (int)_instance);

		dpnnDeviceInfo dInfo;
		dpnnGetDeviceInfo(_instance, &dInfo);
		CCLOG("dpnnDeviceInfo: %f, %d, %d, %f, %f", dInfo.ipd, EYE_BUFFER_SIZE, EYE_BUFFER_SIZE, dInfo.fov_x, dInfo.fov_y);

		for (int eye = 0; eye < EYE_NUM; eye++)
		{
			if (_eyeCamera[eye]) {
				_eyeCamera[eye]->removeFromParent();
			}
			_eyeCamera[eye] = Camera::createPerspective(dInfo.fov_y, (float)EYE_BUFFER_SIZE / (float)EYE_BUFFER_SIZE, 0.1f, 5000.0f);
			_eyeCamera[eye]->setCameraFlag(flag);
			this->addChild(_eyeCamera[eye]);
		}
	}
	);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(backToForegroundListener, -1);

	auto foregroundToBackListener = EventListenerCustom::create(EVENT_COME_TO_BACKGROUND,
		[=](EventCustom*)
	{
		CCLOG("OVRRenderer::vrapi_LeaveVrMode");
		dpnnDeinit(_instance);
		_instance = nullptr;
	}
	);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(foregroundToBackListener, -1);

	this->setCameraMask((unsigned short)flag);
	update(0.0f);
	scheduleUpdate();
	return true;
}

OVRRenderer* OVRRenderer::create(cocos2d::CameraFlag flag)
{
	auto renderer = new OVRRenderer;
	if (!renderer->init(flag)) {
		delete renderer;
		return nullptr;
	}

	renderer->autorelease();
	return renderer;
}

void OVRRenderer::draw(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	if (!_instance) return;
	if (Camera::getVisitingCamera() == _eyeCamera[0] || Camera::getVisitingCamera() == _eyeCamera[1]) {
		_beginRenderCommand.init(-RENDER_ORDER_ID);
		_beginRenderCommand.func = CC_CALLBACK_0(OVRRenderer::onBeginDraw, this);
		renderer->addCommand(&_beginRenderCommand);

		_endRenderCommand.init(RENDER_ORDER_ID);
		_endRenderCommand.func = CC_CALLBACK_0(OVRRenderer::onEndDraw, this);
		renderer->addCommand(&_endRenderCommand);
	}
}

void OVRRenderer::onBeginDraw()
{
	int eye = Camera::getVisitingCamera() == _eyeCamera[0] ? 0 : 1;
	glEnable(GL_SCISSOR_TEST);
	glDepthMask(true);
	int width, height;

	dpnnFramebuffer *frameBuffer = &_frameBuffer[eye];
	dpnnFramebuffer_SetCurrent(frameBuffer);
	width = frameBuffer->Width;
	height = frameBuffer->Height;

	glScissor(0, 0, width, height);
	glViewport(0, 0, width, height);
	glClearColor(0.125f, 0.0f, 0.125f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void OVRRenderer::onEndDraw()
{
	int eye = Camera::getVisitingCamera() == _eyeCamera[0] ? 0 : 1;

	dpnnFramebuffer *frameBuffer = &_frameBuffer[eye];
	// Explicitly clear the border texels to black because OpenGL-ES does not support GL_CLAMP_TO_BORDER.
	{
		// Clear to fully opaque black.
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		// bottom
		glScissor(0, 0, frameBuffer->Width, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		// top
		glScissor(0, frameBuffer->Height - 1, frameBuffer->Width, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		// left
		glScissor(0, 0, 1, frameBuffer->Height);
		glClear(GL_COLOR_BUFFER_BIT);
		// right
		glScissor(frameBuffer->Width - 1, 0, 1, frameBuffer->Height);
		glClear(GL_COLOR_BUFFER_BIT);
	}


	dpnnFramebuffer_Resolve(frameBuffer);

	glDepthMask(false);
	glDisable(GL_SCISSOR_TEST);

	dpnnSetTexture(_instance, (void*)(frameBuffer->texIDs[frameBuffer->TextureSwapIndex]), eye == 0 ? DPNN_EYE_LEFT : DPNN_EYE_RIGHT, DPNN_TW_NONE);

	dpnnFramebuffer_Advance(frameBuffer);

	if (eye == 1) {
		dpnnRecordPose(_instance, DPNN_EYE_COUNT);
		dpnnCompose(_instance);
		dpnnFramebuffer_SetNone();
	}

}

void OVRRenderer::update(float delta)
{
	if (!_instance) return;
	const dpnHmdParms headModelParms = dpnutilDefaultHmdParms();
	//dpnnSensorData tracking;
	//dpnnGetSensorData(OVRHelper::dpInstance, &tracking);

	//CCLOG("dpnnSensorData: (%f, %f, %f), (%f, %f, %f), (%f, %f, %f)", tracking.angular_velocity.x, tracking.angular_velocity.y, tracking.angular_velocity.z
	//	, tracking.linear_acceleration.x, tracking.linear_acceleration.y, tracking.linear_acceleration.z
	//	, tracking.magnetometer.x, tracking.magnetometer.y, tracking.magnetometer.z);

	//const dpnMatrix4 centerEyeViewMatrix = dpnutilGetCenterEyeViewMatrix(&headModelParms, tracking, NULL);

	const dpnnQuarterion pose = dpnnGetPose(_instance);
	const dpnnVector3 position = dpnnGetPosition(_instance);

	for (int eye = 0; eye < EYE_NUM; eye++) {
		//const dpnMatrix4 eyeViewMatrix = dpnutilGetEyeViewMatrix(&headModelParms, &centerEyeViewMatrix, eye);
		//Mat4 viewMat;
		//viewMat.set((const GLfloat *)(dpnutilMatrix4_Transpose(&eyeViewMatrix).M[0]));
		//viewMat.inverse();

		float eyeOffset = (eye ? -0.5f : 0.5f) * headModelParms.ipd;
		Mat4 posMat, rotMat, offsetMat;
		Mat4::createRotation(Quaternion(pose.i, pose.j, pose.k, pose.s), &rotMat);
		Mat4::createTranslation(Vec3(position.x, position.y, position.z), &posMat);
		Mat4::createTranslation(Vec3(eyeOffset, 0.0f, 0.0f), &offsetMat);
		Mat4 viewMat = offsetMat * posMat * rotMat;

		Quaternion quat;
		Vec3 pos;
		viewMat.decompose(nullptr, &quat, &pos);
		quat = _offsetRot * quat;
		Vec3 up = quat * Vec3::UNIT_Y;
		Vec3 forword = quat * -Vec3::UNIT_Z;
		Vec3 shiftedEyePos = _offsetPos + _offsetRot * pos;
		_eyeCamera[eye]->setPosition3D(shiftedEyePos);
		_eyeCamera[eye]->lookAt(shiftedEyePos + forword, up);
		CCLOG("OVRRenderer::camera[%d]-ROT(%f, %f, %f, %f) -POS(%f, %f, %f)", eye, quat.w, quat.x, quat.y, quat.z, pos.x, pos.y, pos.z);
	}

	CCLOG("OVRRenderer::update");
}

void OVRRenderer::setOffsetPos(const cocos2d::Vec3 &pos)
{
	_offsetPos += pos;
}

void OVRRenderer::setOffsetRot(const cocos2d::Quaternion &rot)
{
	_offsetRot *= rot;
}