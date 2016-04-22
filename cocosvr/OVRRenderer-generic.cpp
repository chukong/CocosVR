#include "OVRRenderer-generic.h"

USING_NS_CC;

//////////////////////////////////////////////////////////////////////////////////////////////

#define NUM_MULTI_SAMPLES	4
#define RENDER_ORDER_ID 100.0f

OVRRenderer::OVRRenderer()
{
}

OVRRenderer::~OVRRenderer()
{
}

bool OVRRenderer::init(cocos2d::CameraFlag flag)
{
	CCLOG("OVRRenderer::init");
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
}

void OVRRenderer::onBeginDraw()
{
}


void OVRRenderer::onEndDraw()
{
}

void OVRRenderer::update(float delta)
{
}

void OVRRenderer::setOffsetPos(const cocos2d::Vec3 &pos)
{
}

void OVRRenderer::setOffsetRot(const cocos2d::Quaternion &rot)
{
}
