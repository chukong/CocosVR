#ifndef __OVR_RENDERER_H__
#define __OVR_RENDERER_H__
#include "cocos2d.h"

#define EYE_NUM 2

class OVRRenderer : public cocos2d::Node
{
public:
	OVRRenderer();
	~OVRRenderer();

	virtual void update(float delta) override;
	virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4& transform, uint32_t flags) override;
	void setOffsetPos(const cocos2d::Vec3 &pos);
	void setOffsetRot(const cocos2d::Quaternion &rot);

private:

	void onBeginDraw();
	void onEndDraw();
	virtual void onBeginDrawImp() = 0;
	virtual void onEndDrawImp() = 0;

protected:

	cocos2d::Camera    *_eyeCamera[EYE_NUM];
	cocos2d::Vec3       _offsetPos;
	cocos2d::Quaternion _offsetRot;


	cocos2d::CustomCommand _beginRenderCommand;
	cocos2d::CustomCommand _endRenderCommand;
};

#endif