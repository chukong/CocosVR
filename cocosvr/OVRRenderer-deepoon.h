#ifndef __OVR_RENDERER_DEEPOON_VR_H__
#define __OVR_RENDERER_DEEPOON_VR_H__

#if defined(DEEPOON_VR)

#include "cocos2d.h"
#include <deepoon_sdk.h>
#include <deepoon_sdk_native.h>
#include <deepoon_sdk_utils.h>

#define GL( func )		func;
#define EYE_NUM 2
/*
================================================================================

ovrFramebuffer

================================================================================
*/

typedef struct
{
	int						Width;
	int						Height;
	int						Multisamples;
	int						TextureSwapNum;
	int						TextureSwapIndex;
	GLuint                  *texIDs;
	GLuint				    *DepthBuffers;
	GLuint				    *FrameBuffers;
} dpnnFramebuffer;

static void dpnnFramebuffer_Clear(dpnnFramebuffer * frameBuffer)
{
	frameBuffer->Width = 0;
	frameBuffer->Height = 0;
	frameBuffer->Multisamples = 0;
	frameBuffer->TextureSwapNum = 0;
	frameBuffer->TextureSwapIndex = 0;
	frameBuffer->texIDs = nullptr;
	frameBuffer->DepthBuffers = nullptr;
	frameBuffer->FrameBuffers = nullptr;
}

static bool dpnnFramebuffer_Create(dpnnFramebuffer * frameBuffer, const int width, const int height, const int multisamples)
{
	frameBuffer->Width = width;
	frameBuffer->Height = height;
	frameBuffer->Multisamples = multisamples;

	frameBuffer->TextureSwapNum = 3;
	frameBuffer->texIDs = (GLuint *)malloc(frameBuffer->TextureSwapNum * sizeof(GLuint));
	frameBuffer->DepthBuffers = (GLuint *)malloc(frameBuffer->TextureSwapNum * sizeof(GLuint));
	frameBuffer->FrameBuffers = (GLuint *)malloc(frameBuffer->TextureSwapNum * sizeof(GLuint));

	//PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC glRenderbufferStorageMultisampleEXT =
	//	(PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC)eglGetProcAddress("glRenderbufferStorageMultisampleEXT");
	//PFNGLFRAMEBUFFERTEXTURE2DMULTISAMPLEEXTPROC glFramebufferTexture2DMultisampleEXT =
	//	(PFNGLFRAMEBUFFERTEXTURE2DMULTISAMPLEEXTPROC)eglGetProcAddress("glFramebufferTexture2DMultisampleEXT");

	for (int i = 0; i < frameBuffer->TextureSwapNum; i++) {
		// Create the color buffer texture.
		GL(glGenTextures(1, &frameBuffer->texIDs[i]));
		GL(glBindTexture(GL_TEXTURE_2D, frameBuffer->texIDs[i]));
		GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, NULL));
		GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GL(glBindTexture(GL_TEXTURE_2D, 0));

		//if (multisamples > 1 && glRenderbufferStorageMultisampleEXT != NULL && glFramebufferTexture2DMultisampleEXT != NULL)
		//{
		//	CCLOG("OVRHelper::Create multisampled buffers");
		//	// Create multisampled depth buffer.
		//	GL(glGenRenderbuffers(1, &frameBuffer->DepthBuffers[i]));
		//	GL(glBindRenderbuffer(GL_RENDERBUFFER, frameBuffer->DepthBuffers[i]));
		//	GL(glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER, multisamples, GL_DEPTH_COMPONENT24, width, height));
		//	GL(glBindRenderbuffer(GL_RENDERBUFFER, 0));

		//	// Create the frame buffer.
		//	GL(glGenFramebuffers(1, &frameBuffer->FrameBuffers[i]));
		//	GL(glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer->FrameBuffers[i]));
		//	GL(glFramebufferTexture2DMultisampleEXT(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameBuffer->texIDs[i], 0, multisamples));
		//	GL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, frameBuffer->DepthBuffers[i]));
		//	GL(GLenum renderFramebufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER));
		//	GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		//	if (renderFramebufferStatus != GL_FRAMEBUFFER_COMPLETE)
		//	{
		//		CCLOG("OVRHelper::Incomplete frame buffer object: %s", GlFrameBufferStatusString(renderFramebufferStatus));
		//		return false;
		//	}
		//}
		//else
		{
			CCLOG("OVRHelper::Create buffers");
			// Create depth buffer.
			GL(glGenRenderbuffers(1, &frameBuffer->DepthBuffers[i]));
			GL(glBindRenderbuffer(GL_RENDERBUFFER, frameBuffer->DepthBuffers[i]));
			GL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24_OES, width, height));
			GL(glBindRenderbuffer(GL_RENDERBUFFER, 0));

			// Create the frame buffer.
			GL(glGenFramebuffers(1, &frameBuffer->FrameBuffers[i]));
			GL(glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer->FrameBuffers[i]));
			GL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, frameBuffer->DepthBuffers[i]));
			GL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameBuffer->texIDs[i], 0));
			GL(GLenum renderFramebufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER));
			GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
			if (renderFramebufferStatus != GL_FRAMEBUFFER_COMPLETE)
			{
				CCLOG("OVRHelper::Incomplete frame buffer object");
				return false;
			}
		}
	}

	return true;
}

static void dpnnFramebuffer_Destroy(dpnnFramebuffer * frameBuffer)
{
	GL(glDeleteTextures(frameBuffer->TextureSwapNum, frameBuffer->texIDs));
	GL(glDeleteFramebuffers(frameBuffer->TextureSwapNum, frameBuffer->FrameBuffers));
	GL(glDeleteRenderbuffers(frameBuffer->TextureSwapNum, frameBuffer->DepthBuffers));

	free(frameBuffer->texIDs);
	free(frameBuffer->DepthBuffers);
	free(frameBuffer->FrameBuffers);

	dpnnFramebuffer_Clear(frameBuffer);
}

static void dpnnFramebuffer_SetCurrent(dpnnFramebuffer * frameBuffer)
{
	GL(glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer->FrameBuffers[frameBuffer->TextureSwapIndex]));
}

static void dpnnFramebuffer_SetNone()
{
	GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

static void dpnnFramebuffer_Resolve(dpnnFramebuffer * frameBuffer)
{
	// Discard the depth buffer, so the tiler won't need to write it back out to memory.
	const GLenum depthAttachment[1] = { GL_DEPTH_ATTACHMENT };
	//glInvalidateFramebuffer(GL_FRAMEBUFFER, 1, depthAttachment);

	// Flush this frame worth of commands.
	glFlush();
}

static void dpnnFramebuffer_Advance(dpnnFramebuffer * frameBuffer)
{
	// Advance to the next texture from the set.
	frameBuffer->TextureSwapIndex = (frameBuffer->TextureSwapIndex + 1) % frameBuffer->TextureSwapNum;
}

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

	dpnnFramebuffer _frameBuffer[EYE_NUM];
	dpnnInstance    _instance;

	cocos2d::CustomCommand _beginRenderCommand;
	cocos2d::CustomCommand _endRenderCommand;
};

#endif // defined(DEEPOON_VR)

#endif