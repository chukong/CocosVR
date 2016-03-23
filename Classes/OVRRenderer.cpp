#include "OVRRenderer.h"

USING_NS_CC;

#define RENDER_ORDER_ID 10000

OVRRenderer::OVRRenderer()
{
	for (int eye = 0; eye < EYE_NUM; ++eye) {
		_eyeCamera[eye] = nullptr;
	}
}

OVRRenderer::~OVRRenderer()
{
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
	onBeginDrawImp();
}


void OVRRenderer::onEndDraw()
{
	onEndDrawImp();
}

void OVRRenderer::update(float delta)
{
}

void OVRRenderer::setOffsetPos(const cocos2d::Vec3 &pos)
{
	_offsetPos += pos;
}

void OVRRenderer::setOffsetRot(const cocos2d::Quaternion &rot)
{
	_offsetRot *= rot;
}
