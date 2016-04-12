#ifndef __OVR_RENDERER_GENERIC_VR_H__
#define __OVR_RENDERER_GENERIC_VR_H__
#include "cocos2d.h"

class OVRRenderer : public cocos2d::Node
{
public:
	OVRRenderer();
	~OVRRenderer();

	static OVRRenderer* create(cocos2d::CameraFlag flag);

	virtual void update(float delta) override;
	virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4& transform, uint32_t flags) override;
	void setOffsetPos(const cocos2d::Vec3 &pos);
	void setOffsetRot(const cocos2d::Quaternion &rot);

private:

	bool init(cocos2d::CameraFlag flag);
	void onBeginDraw();
	void onEndDraw();

private:
};


#endif