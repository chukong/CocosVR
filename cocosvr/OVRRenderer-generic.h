#ifndef __OVR_RENDERER_GENERIC_VR_H__
#define __OVR_RENDERER_GENERIC_VR_H__
#include "cocos2d.h"

class DistortionMesh;
class HeadMountedDisplay;

struct EyeViewport
{
    float x;
    float y;
    float width;
    float height;
    float eyeX;
    float eyeY;
};

class OVRRenderer : public cocos2d::Node
{
    static const int EYE_NUM = 2;
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
    int setupRenderTextureAndRenderbuffer(int width, int height);
	void onBeginDraw();
	void onEndDraw();
    void setupGLProgram();
    void renderDistortionMesh(DistortionMesh *mesh, GLint textureID);
    void updateTextureAndDistortionMesh();
    DistortionMesh* createDistortionMesh(const EyeViewport& eyeViewport, float textureWidthTanAngle, float textureHeightTanAngle, float xEyeOffsetTanAngleScreen, float yEyeOffsetTanAngleScreen);

    cocos2d::Camera* _eyeCamera[EYE_NUM];
    cocos2d::Vec3       _offsetPos;
    cocos2d::Quaternion _offsetRot;

    GLint _framebufferId;
    GLint _textureId;
    GLint _renderbufferId;
    int _viewport[4];
    GLboolean _cullFaceEnabled;
    GLboolean _scissorTestEnabled;
    HeadMountedDisplay* _headMountedDisplay;
    DistortionMesh* _leftEyeDistortionMesh;
    DistortionMesh* _rightEyeDistortionMesh;
    EyeViewport _leftEyeViewport;
    EyeViewport _rightEyeViewport;
    float _metersPerTanAngle;

    cocos2d::CustomCommand _beginRenderCommand;
    cocos2d::CustomCommand _endRenderCommand;
};


#endif