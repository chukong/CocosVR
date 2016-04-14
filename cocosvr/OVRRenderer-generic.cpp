#include "OVRRenderer-generic.h"
#include "platform/CCGL.h"

USING_NS_CC;

//////////////////////////////////////////////////////////////////////////////////////////////
#define NUM_MULTI_SAMPLES	4
#define RENDER_ORDER_ID 100.0f
const int EYE_BUFFER_SIZE = 1024;

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

OVRRenderer::OVRRenderer()
: _textureId(-1)
, _framebufferId(-1)
, _renderbufferId(-1)
{
    for (int eye = 0; eye < EYE_NUM; eye++)
        _eyeCamera[eye] = nullptr;
}

OVRRenderer::~OVRRenderer()
{
}

bool OVRRenderer::init(cocos2d::CameraFlag flag)
{
    CCLOG("OVRRenderer::init");

    for (int eye = 0; eye < EYE_NUM; eye++)
    {
        if (_eyeCamera[eye]) {
            _eyeCamera[eye]->removeFromParent();
        }
        _eyeCamera[eye] = Camera::createPerspective(60,
                                                    (float)EYE_BUFFER_SIZE / (float)EYE_BUFFER_SIZE,
                                                    0.1f, 5000.0f);
        _eyeCamera[eye]->setCameraFlag(flag);
        this->addChild(_eyeCamera[eye]);
    }

    this->setCameraMask((unsigned short)flag);
    update(0.0f);
    scheduleUpdate();
    return true;
}

int OVRRenderer::setupRenderTextureAndRenderbuffer(int width, int height)
{
    if (_textureId != -1)
        glDeleteTextures(1, (GLuint*)&_textureId);

    if (_renderbufferId != -1)
        glDeleteRenderbuffers(1, (GLuint*)&_renderbufferId);

    if (_framebufferId != -1)
        glDeleteFramebuffers(1, (GLuint*)&_framebufferId);

    // create texture
    glGenTextures(1, (GLuint*)&_textureId);
    glBindTexture(GL_TEXTURE_2D, _textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL);

    CHECK_GL_ERROR_DEBUG();

    glGenRenderbuffers(1, (GLuint*)&_renderbufferId);
    glBindRenderbuffer(GL_RENDERBUFFER, _renderbufferId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);

    CHECK_GL_ERROR_DEBUG();

    glGenFramebuffers(1, (GLuint*)&_framebufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, _framebufferId);

    CHECK_GL_ERROR_DEBUG();

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _textureId, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _renderbufferId);

    GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    CCASSERT(status == GL_FRAMEBUFFER_COMPLETE, "Invalid framebuffer");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    CHECK_GL_ERROR_DEBUG();

    return _framebufferId;
}

void OVRRenderer::draw(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
    CCLOG("OVRRenderer::draw");
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
    CCLOG("OVRRenderer::onBeginDraw");
    glBindFramebuffer(GL_FRAMEBUFFER, _framebufferId);

//    int eye = Camera::getVisitingCamera() == _eyeCamera[0] ? 0 : 1;
    glEnable(GL_SCISSOR_TEST);
    glDepthMask(true);

    glBindFramebuffer(GL_FRAMEBUFFER, _framebufferId);

    const int width = 512;
    const int height = 512;

    glScissor(0, 0, width, height);
    glViewport(0, 0, width, height);
    glClearColor(0.125f, 0.0f, 0.125f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void OVRRenderer::onEndDraw()
{
    const int width = 512;
    const int height = 512;

    //glBindFramebuffer(GL_FRAMEBUFFER, this->originalFramebufferId);
    glViewport(0, 0, width, height);

    glGetIntegerv(GL_VIEWPORT, _viewport);

    _cullFaceEnabled = glIsEnabled(GL_CULL_FACE);
    _scissorTestEnabled = glIsEnabled(GL_SCISSOR_TEST);

    glDisable(GL_CULL_FACE);
    glDisable(GL_SCISSOR_TEST);

    glClearColor(0.0F, 0.0F, 0.0F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Set texture
    GL::bindTexture2D(_textureId);
    _glProgramState->apply(Mat4::IDENTITY);

    glEnable(GL_SCISSOR_TEST);

    glScissor(0, 0, width / 2, height);

//    this->renderDistortionMesh(this->leftEyeDistortionMesh);

    glScissor(width / 2, 0, width / 2, height);

//    this->renderDistortionMesh(this->rightEyeDistortionMesh);

//    glDisableVertexAttribArray(this->programHolder->aPosition);
//    glDisableVertexAttribArray(this->programHolder->aVignette);
//    glDisableVertexAttribArray(this->programHolder->aTextureCoord);
//    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    if (_cullFaceEnabled)
        glEnable(GL_CULL_FACE);
    if (_scissorTestEnabled)
        glEnable(GL_SCISSOR_TEST);
    else
        glDisable(GL_SCISSOR_TEST);

    glViewport(_viewport[0], _viewport[1], _viewport[2], _viewport[3]);
    
    CHECK_GL_ERROR_DEBUG();
}

void OVRRenderer::update(float delta)
{
    CCLOG("OVRRenderer::update");
}

void OVRRenderer::setOffsetPos(const cocos2d::Vec3 &pos)
{
}

void OVRRenderer::setOffsetRot(const cocos2d::Quaternion &rot)
{
}
