#include "OVRRenderer-gearvr.h"

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
	ovrJava java;
	java.Vm = OVRHelper::javaVM;
	java.Vm->AttachCurrentThread(&java.Env, NULL);
	java.ActivityObject = OVRHelper::activity;
	SystemActivities_Init(&java);
	const ovrInitParms initParms = vrapi_DefaultInitParms(&java);
	int32_t initResult = vrapi_Initialize(&initParms);
	if (initResult != VRAPI_INITIALIZE_SUCCESS)
	{
		char const * msg = initResult == VRAPI_INITIALIZE_PERMISSIONS_ERROR ?
			"Thread priority security exception. Make sure the APK is signed." :
			"VrApi initialization error.";
		SystemActivities_DisplayError(&java, SYSTEM_ACTIVITIES_FATAL_ERROR_OSIG, __FILE__, msg);
	}
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
	: _ovr(nullptr)
	, _frameIndex(1)
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
	// Create an EGLContext and get the suggested FOV and suggested
	// resolution to setup a projection matrix and eye texture swap chains.
	_java.Vm = OVRHelper::javaVM;
	_java.Vm->AttachCurrentThread(&_java.Env, NULL);
	_java.ActivityObject = OVRHelper::activity;

	const float suggestedEyeFovDegreesX = vrapi_GetSystemPropertyFloat(&_java, VRAPI_SYS_PROP_SUGGESTED_EYE_FOV_DEGREES_X);
	const float suggestedEyeFovDegreesY = vrapi_GetSystemPropertyFloat(&_java, VRAPI_SYS_PROP_SUGGESTED_EYE_FOV_DEGREES_Y);
	const float halfWidth = VRAPI_ZNEAR * tanf(suggestedEyeFovDegreesX * (VRAPI_PI / 180.0f * 0.5f));
	const float halfHeight = VRAPI_ZNEAR * tanf(suggestedEyeFovDegreesY * (VRAPI_PI / 180.0f * 0.5f));

	for (int eye = 0; eye < EYE_NUM; eye++)
	{
		_eyeCamera[eye] = Camera::createPerspective(suggestedEyeFovDegreesY, halfWidth / halfHeight, VRAPI_ZNEAR, 5000.0f);
		_eyeCamera[eye]->setCameraFlag(flag);
		this->addChild(_eyeCamera[eye]);
	}

	// Setup a projection matrix based on the 'ovrHmdInfo'.
	_projectionMatrix = ovrMatrix4f_CreateProjectionFov(suggestedEyeFovDegreesX,
		suggestedEyeFovDegreesY,
		0.0f, 0.0f, VRAPI_ZNEAR, 5000.0f);

	// Create the frame buffers.
	CCLOG("OVRRenderer::ovrFramebuffer_Create");
	for (int eye = 0; eye < EYE_NUM; eye++)
	{
		ovrFramebuffer_Clear(&_frameBuffer[eye]);
		ovrFramebuffer_Create(&_frameBuffer[eye],
			VRAPI_TEXTURE_FORMAT_8888,
			vrapi_GetSystemPropertyInt(&_java, VRAPI_SYS_PROP_SUGGESTED_EYE_TEXTURE_WIDTH),
			vrapi_GetSystemPropertyInt(&_java, VRAPI_SYS_PROP_SUGGESTED_EYE_TEXTURE_HEIGHT),
			NUM_MULTI_SAMPLES);
	}

	auto backToForegroundListener = EventListenerCustom::create(EVENT_COME_TO_FOREGROUND,
		[=](EventCustom*)
	{
		CCLOG("OVRRenderer::vrapi_EnterVrMode");
		ovrModeParms modeParms = vrapi_DefaultModeParms(&_java);
		_ovr = vrapi_EnterVrMode(&modeParms);
	}
	);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(backToForegroundListener, -1);

	auto foregroundToBackListener = EventListenerCustom::create(EVENT_COME_TO_BACKGROUND,
		[=](EventCustom*)
	{
		CCLOG("OVRRenderer::vrapi_LeaveVrMode");
		vrapi_LeaveVrMode(_ovr);
		_ovr = nullptr;
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
	if (!_ovr) return;
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
	ovrFramebuffer * frameBuffer = &_frameBuffer[eye];
	ovrFramebuffer_SetCurrent(frameBuffer);
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

	ovrFramebuffer * frameBuffer = &_frameBuffer[eye];
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


	ovrFramebuffer_Resolve(frameBuffer);

	_frameParams.Layers[VRAPI_FRAME_LAYER_TYPE_WORLD].Textures[eye].ColorTextureSwapChain = frameBuffer->ColorTextureSwapChain;
	_frameParams.Layers[VRAPI_FRAME_LAYER_TYPE_WORLD].Textures[eye].TextureSwapChainIndex = frameBuffer->TextureSwapChainIndex;
	_frameParams.Layers[VRAPI_FRAME_LAYER_TYPE_WORLD].Textures[eye].TexCoordsFromTanAngles = ovrMatrix4f_TanAngleMatrixFromProjection(&_projectionMatrix);
	_frameParams.Layers[VRAPI_FRAME_LAYER_TYPE_WORLD].Textures[eye].HeadPose = _tracking.HeadPose;
	ovrFramebuffer_Advance(frameBuffer);

	glDepthMask(false);
	glDisable(GL_SCISSOR_TEST);

	if (eye == 1){
		ovrFramebuffer_SetNone();
		vrapi_SubmitFrame(_ovr, &_frameParams);
	}

}

void OVRRenderer::update(float delta)
{
	if (!_ovr) return;
	_frameIndex++;
	// Get the HMD pose, predicted for the middle of the time period during which
	// the new eye images will be displayed. The number of frames predicted ahead
	// depends on the pipeline depth of the engine and the synthesis rate.
	// The better the prediction, the less black will be pulled in at the edges.
	const double predictedDisplayTime = vrapi_GetPredictedDisplayTime(_ovr, _frameIndex);
	const ovrTracking baseTracking = vrapi_GetPredictedTracking(_ovr, predictedDisplayTime);
	// Apply the head-on-a-stick model if there is no positional tracking.
	const ovrHeadModelParms headModelParms = vrapi_DefaultHeadModelParms();
	_tracking = vrapi_ApplyHeadModel(&headModelParms, &baseTracking);
	const ovrMatrix4f centerEyeViewMatrix = vrapi_GetCenterEyeViewMatrix(&headModelParms, &_tracking, NULL);

	for (int eye = 0; eye < EYE_NUM; eye++){
		const ovrMatrix4f eyeViewMatrix = vrapi_GetEyeViewMatrix(&headModelParms, &centerEyeViewMatrix, eye);
		Mat4 viewMat;
		viewMat.set((const GLfloat *)(ovrMatrix4f_Transpose(&eyeViewMatrix).M[0]));
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

	_frameParams = vrapi_DefaultFrameParms(&_java, VRAPI_FRAME_INIT_DEFAULT, vrapi_GetTimeInSeconds(), NULL);
	_frameParams.FrameIndex = _frameIndex;
	_frameParams.MinimumVsyncs = 1;
	_frameParams.PerformanceParms = vrapi_DefaultPerformanceParms();
	_frameParams.Layers[VRAPI_FRAME_LAYER_TYPE_WORLD].Flags |= VRAPI_FRAME_LAYER_FLAG_CHROMATIC_ABERRATION_CORRECTION;

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