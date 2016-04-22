#ifndef __OVR_RENDERER_CARDBOARD_VR_H__
#define __OVR_RENDERER_CARDBOARD_VR_H__

#if defined(CARDBOARD_VR)

#include "cocos2d.h"
#include "CbApi.h"
#include "CbApi_Helpers.h"

#define EYE_NUM CB_EYE_NUM

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

	cocos2d::Camera *_eyeCamera[EYE_NUM];
	cocos2d::Vec3       _offsetPos;
	cocos2d::Quaternion _offsetRot;

	cbJava         _java;
	cbHeadMountedDisplay _headMountDisplay;
	cbEyes         _eyes;


	cocos2d::CustomCommand _beginRenderCommand;
	cocos2d::CustomCommand _endRenderCommand;
};
#endif // defined(CARDBOARD_VR)

#endif