#include "OVRRenderer-cardboard.h"

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

jstring cocos_android_app_onLoadLibraryName(JNIEnv* env) {
	CCLOG("cocos_android_app_onLoadLibraryName");
	return NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////////

#define NUM_MULTI_SAMPLES	4
#define RENDER_ORDER_ID 100.0f

const int EYE_BUFFER_SIZE = 1024;

OVRRenderer::OVRRenderer()
{
	for (int eye = 0; eye < EYE_NUM; eye++)
	{
		_eyeCamera[eye] = nullptr;
	}
}

OVRRenderer::~OVRRenderer()
{
}

bool OVRRenderer::init(cocos2d::CameraFlag flag)
{
	// Create an EGLContext and get the suggested FOV and suggested
	// resolution to setup a projection matrix and eye texture swap chains.
	_java.Vm = OVRHelper::javaVM;
	_java.Vm->AttachCurrentThread(&_java.Env, NULL);
	_java.ActivityObject = OVRHelper::activity;
    if (!cbapi_Initialize(&_java, &_headMountDisplay)){
        CCLOG("cbapi_Initialize Failed!");
        return false;
    }

	_eyes = cbapi_GetEyes();

	for (int eye = 0; eye < EYE_NUM; eye++)
	{
	    float suggestedEyeFovDegreesX = _eyes.eyeParams[eye].fov.left + _eyes.eyeParams[eye].fov.right;
	    float suggestedEyeFovDegreesY = _eyes.eyeParams[eye].fov.bottom + _eyes.eyeParams[eye].fov.top;
		const float halfWidth = CBAPI_ZNEAR * tanf(suggestedEyeFovDegreesX * (CBAPI_PI / 180.0f * 0.5f));
        const float halfHeight = CBAPI_ZNEAR * tanf(suggestedEyeFovDegreesY * (CBAPI_PI / 180.0f * 0.5f));
		_eyeCamera[eye] = Camera::createPerspective(suggestedEyeFovDegreesY, halfWidth / halfHeight, CBAPI_ZNEAR, 5000.0f);
		_eyeCamera[eye]->setCameraFlag(flag);
		this->addChild(_eyeCamera[eye]);
	}

	auto backToForegroundListener = EventListenerCustom::create(EVENT_COME_TO_FOREGROUND,
		[=](EventCustom*)
	{
		CCLOG("OVRRenderer::cbapi_EnterVrMode");
		cbapi_EnterVrMode();
	}
	);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(backToForegroundListener, -1);

	auto foregroundToBackListener = EventListenerCustom::create(EVENT_COME_TO_BACKGROUND,
		[=](EventCustom*)
	{
		CCLOG("OVRRenderer::cbapi_LeaveVrMode");
		cbapi_LeaveVrMode();
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
	cbapi_beforeDrawFrame();
	auto vp = _eyes.eyeParams[eye].viewport;

	glEnable(GL_SCISSOR_TEST);
	glDepthMask(true);

	CCLOG("VIEWPORT:(%d, %d, %d, %d)", vp.x, vp.y, vp.width, vp.height);

	glScissor(vp.x, vp.y, vp.width, vp.height);
	glViewport(vp.x, vp.y, vp.width, vp.height);
	glClearColor(0.125f, 0.0f, 0.125f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void OVRRenderer::onEndDraw()
{
	int eye = Camera::getVisitingCamera() == _eyeCamera[0] ? 0 : 1;

	glDepthMask(false);
	glDisable(GL_SCISSOR_TEST);

	if (eye == 1){
	    cbapi_afterDrawFrame();
	}

}

void OVRRenderer::update(float delta)
{

	const cbMatrix4 headViewMatrix = cbapi_getLastHeadView();

	for (int eye = 0; eye < EYE_NUM; eye++){
		const cbMatrix4 eyeViewMatrix = cbapi_GetEyeViewMatrix(&_headMountDisplay.device, &headViewMatrix, eye);
		Mat4 viewMat;
		viewMat.set((const GLfloat *)(cbMatrix4_Transpose(&eyeViewMatrix).M[0]));
		viewMat.inverse();

		Quaternion quat;
		Vec3 pos;
		viewMat.decompose(nullptr, &quat, &pos);
		quat = _offsetRot * quat;
		Vec3 up = quat * Vec3::UNIT_Y;
		Vec3 forword = quat * -Vec3::UNIT_Z;
		Vec3 shiftedEyePos = _offsetPos + _offsetRot * pos;
		_eyeCamera[eye]->setPosition3D(shiftedEyePos);
		_eyeCamera[eye]->lookAt(shiftedEyePos + forword, up);
		//CCLOG("OVRRenderer::camera[%d]-ROT(%f, %f, %f, %f) -POS(%f, %f, %f)", eye, quat.w, quat.x, quat.y, quat.z, pos.x, pos.y, pos.z);
	}

	//CCLOG("OVRRenderer::update");
}

void OVRRenderer::setOffsetPos(const cocos2d::Vec3 &pos)
{
	_offsetPos += pos;
}

void OVRRenderer::setOffsetRot(const cocos2d::Quaternion &rot)
{
	_offsetRot *= rot;
}