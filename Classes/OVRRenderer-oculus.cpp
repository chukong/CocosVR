#include "OVRRenderer.h"

USING_NS_CC;

#define RENDER_ORDER_ID 100

OVRRenderer::OVRRenderer()
	: _mirrorFBO(0)
	, _mirrorTexture(nullptr)
	, _offsetPos(Vec3::ZERO)
{
	for (int eye = 0; eye < 2; ++eye) {
		_eyeRenderTexture[eye] = nullptr;
		_eyeDepthBuffer[eye] = nullptr;
		_eyeCamera[eye] = nullptr;
	}
}

OVRRenderer::~OVRRenderer()
{
	if (_mirrorFBO) glDeleteFramebuffers(1, &_mirrorFBO);
	if (_mirrorTexture) ovr_DestroyMirrorTexture(_HMD, reinterpret_cast<ovrTexture*>(_mirrorTexture));
	for (int eye = 0; eye < 2; ++eye)
	{
		delete _eyeRenderTexture[eye];
		delete _eyeDepthBuffer[eye];
	}
	ovr_Destroy(_HMD);
	ovr_Shutdown();
	OVR::System::Destroy();
}

bool OVRRenderer::init(cocos2d::CameraFlag flag)
{
	OVR::System::Init();
	// Initializes LibOVR, and the Rift
	ovrResult result = ovr_Initialize(nullptr);
	if (!OVR_SUCCESS(result)) {
		CCLOG("Failed to initialize libOVR.");
		return false;
	};

	ovrGraphicsLuid luid;
	result = ovr_Create(&_HMD, &luid);
	if (!OVR_SUCCESS(result)) {
		CCLOG("Failed to create HMD.");
		return false;
	}
	ovrHmdDesc hmdDesc = ovr_GetHmdDesc(_HMD);

	// Make eye render buffers
	for (int eye = 0; eye < 2; ++eye){
		ovrSizei idealTextureSize = ovr_GetFovTextureSize(_HMD, ovrEyeType(eye), hmdDesc.DefaultEyeFov[eye], 1);
		_eyeRenderTexture[eye] = new TextureBuffer(_HMD, true, true, idealTextureSize, 1, nullptr, 1);
		_eyeDepthBuffer[eye] = new DepthBuffer(_eyeRenderTexture[eye]->GetSize(), 0);

		if (!_eyeRenderTexture[eye]->TextureSet){
			CCLOG("Failed to create texture.");
			return false;
		}
	}

	// Create mirror texture and an FBO used to copy mirror texture to back buffer
	result = ovr_CreateMirrorTextureGL(_HMD, GL_SRGB8_ALPHA8, Director::getInstance()->getWinSize().width, Director::getInstance()->getWinSize().height, reinterpret_cast<ovrTexture**>(&_mirrorTexture));
	if (!OVR_SUCCESS(result)){
		CCLOG("Failed to create mirror texture.");
		return false;
	}

	// Configure the mirror read buffer
	glGenFramebuffers(1, &_mirrorFBO);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, _mirrorFBO);
	glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _mirrorTexture->OGL.TexId, 0);
	glFramebufferRenderbuffer(GL_READ_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

	_eyeRenderDesc[0] = ovr_GetRenderDesc(_HMD, ovrEye_Left, hmdDesc.DefaultEyeFov[0]);
	_eyeRenderDesc[1] = ovr_GetRenderDesc(_HMD, ovrEye_Right, hmdDesc.DefaultEyeFov[1]);

	for (int eye = 0; eye < 2; ++eye) {
		float fov = CC_RADIANS_TO_DEGREES(atan(hmdDesc.DefaultEyeFov[eye].UpTan) + atan(hmdDesc.DefaultEyeFov[eye].DownTan));
		float aspectRadio = (hmdDesc.DefaultEyeFov[eye].LeftTan + hmdDesc.DefaultEyeFov[eye].RightTan) / (hmdDesc.DefaultEyeFov[eye].UpTan + hmdDesc.DefaultEyeFov[eye].DownTan);
		_eyeCamera[eye] = Camera::createPerspective(fov, aspectRadio, 0.2f, 5000.0f);
		_eyeCamera[eye]->setCameraFlag(flag);
		//_eyeCamera[eye]->setDepth(eye + 1);
		this->addChild(_eyeCamera[eye]);
	}

	this->setCameraMask((unsigned short)flag);
	_ld.Header.Type  = ovrLayerType_EyeFov;
	_ld.Header.Flags = ovrLayerFlag_TextureOriginAtBottomLeft;   // Because OpenGL.

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
	glDepthMask(true);
	int eye = Camera::getVisitingCamera() == _eyeCamera[0] ? 0 : 1;
	_eyeRenderTexture[eye]->SetAndClearRenderSurface(_eyeDepthBuffer[eye]);
}


void OVRRenderer::onEndDraw()
{
	int eye = Camera::getVisitingCamera() == _eyeCamera[0] ? 0 : 1;
	_eyeRenderTexture[eye]->UnsetRenderSurface();
	//_eyeRenderTexture[eye]->BlitFramebuffer(_mirrorTexture->OGL.Header.TextureSize.w, _mirrorTexture->OGL.Header.TextureSize.h);

	if (eye == 1) {
		// Set up positional data.
		ovrViewScaleDesc viewScaleDesc;
		viewScaleDesc.HmdSpaceToWorldScaleInMeters = 1.0f;
		viewScaleDesc.HmdToEyeViewOffset[0] = _eyeRenderDesc[0].HmdToEyeViewOffset;
		viewScaleDesc.HmdToEyeViewOffset[1] = _eyeRenderDesc[1].HmdToEyeViewOffset;
		ovrLayerHeader* layers = &_ld.Header;
		ovrResult result = ovr_SubmitFrame(_HMD, 0, &viewScaleDesc, &layers, 1);
		if (!OVR_SUCCESS(result)) {
			CCLOG("Failed to submit frame.");
		}
		// Blit mirror texture to back buffer
		glBindFramebuffer(GL_READ_FRAMEBUFFER, _mirrorFBO);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		GLint w = _mirrorTexture->OGL.Header.TextureSize.w;
		GLint h = _mirrorTexture->OGL.Header.TextureSize.h;
		glBlitFramebuffer(0, h, w, 0,
			0, 0, w, h,
			GL_COLOR_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	}
	glDepthMask(false);
}

void OVRRenderer::update(float delta)
{
		// Get eye poses, feeding in correct IPD offset
	ovrVector3f               ViewOffset[2] = { _eyeRenderDesc[0].HmdToEyeViewOffset, _eyeRenderDesc[1].HmdToEyeViewOffset };
	ovrPosef                  EyeRenderPose[2];

	double           ftiming = ovr_GetPredictedDisplayTime(_HMD, 0);
	// Keeping sensorSampleTime as close to ovr_GetTrackingState as possible - fed into the layer
	double           sensorSampleTime = ovr_GetTimeInSeconds();
	ovrTrackingState hmdState = ovr_GetTrackingState(_HMD, ftiming, ovrTrue);
	ovr_CalcEyePoses(hmdState.HeadPose.ThePose, ViewOffset, EyeRenderPose);

	for (int eye = 0; eye < 2; ++eye)
	{
		Quaternion quat(EyeRenderPose[eye].Orientation.x, EyeRenderPose[eye].Orientation.y, EyeRenderPose[eye].Orientation.z, EyeRenderPose[eye].Orientation.w);
		quat = _offsetRot * quat;
		Vec3 up = quat * Vec3::UNIT_Y;
		Vec3 forword = quat * -Vec3::UNIT_Z;

		Vec3 shiftedEyePos = _offsetPos + _offsetRot * Vec3(EyeRenderPose[eye].Position.x, EyeRenderPose[eye].Position.y, EyeRenderPose[eye].Position.z);
		_eyeCamera[eye]->setPosition3D(shiftedEyePos);
		_eyeCamera[eye]->lookAt(shiftedEyePos + forword, up);

		_eyeRenderTexture[eye]->TextureSet->CurrentIndex = (_eyeRenderTexture[eye]->TextureSet->CurrentIndex + 1) % _eyeRenderTexture[eye]->TextureSet->TextureCount;
		_ld.ColorTexture[eye] = _eyeRenderTexture[eye]->TextureSet;
		_ld.Viewport[eye] = OVR::Recti(_eyeRenderTexture[eye]->GetSize());
		_ld.Fov[eye] = _eyeRenderDesc[eye].Fov;
		_ld.RenderPose[eye] = EyeRenderPose[eye];
		_ld.SensorSampleTime = sensorSampleTime;
	}
}

void OVRRenderer::setOffsetPos(const cocos2d::Vec3 &pos)
{
	_offsetPos += pos;
}

void OVRRenderer::setOffsetRot(const cocos2d::Quaternion &rot)
{
	_offsetRot *= rot;
}
