#ifndef __OVR_RENDERER_GEAR_VR_H__
#define __OVR_RENDERER_GEAR_VR_H__

#if defined(GEAR_VR)

#include "cocos2d.h"
#include <VrApi.h>
#include <VrApi_Helpers.h>
#include <SystemActivities.h>

#define GL( func )		func;
#define EYE_NUM VRAPI_FRAME_LAYER_EYE_MAX
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
	int						TextureSwapChainLength;
	int						TextureSwapChainIndex;
	ovrTextureSwapChain *	ColorTextureSwapChain;
	GLuint *				DepthBuffers;
	GLuint *				FrameBuffers;
} ovrFramebuffer;

static void ovrFramebuffer_Clear(ovrFramebuffer * frameBuffer)
{
	frameBuffer->Width = 0;
	frameBuffer->Height = 0;
	frameBuffer->Multisamples = 0;
	frameBuffer->TextureSwapChainLength = 0;
	frameBuffer->TextureSwapChainIndex = 0;
	frameBuffer->ColorTextureSwapChain = NULL;
	frameBuffer->DepthBuffers = NULL;
	frameBuffer->FrameBuffers = NULL;
}

static bool ovrFramebuffer_Create(ovrFramebuffer * frameBuffer, const ovrTextureFormat colorFormat, const int width, const int height, const int multisamples)
{
	frameBuffer->Width = width;
	frameBuffer->Height = height;
	frameBuffer->Multisamples = multisamples;

	frameBuffer->ColorTextureSwapChain = vrapi_CreateTextureSwapChain(VRAPI_TEXTURE_TYPE_2D, colorFormat, width, height, 1, true);
	frameBuffer->TextureSwapChainLength = vrapi_GetTextureSwapChainLength(frameBuffer->ColorTextureSwapChain);
	frameBuffer->DepthBuffers = (GLuint *)malloc(frameBuffer->TextureSwapChainLength * sizeof(GLuint));
	frameBuffer->FrameBuffers = (GLuint *)malloc(frameBuffer->TextureSwapChainLength * sizeof(GLuint));

	//PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC glRenderbufferStorageMultisampleEXT =
	//	(PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC)eglGetProcAddress("glRenderbufferStorageMultisampleEXT");
	//PFNGLFRAMEBUFFERTEXTURE2DMULTISAMPLEEXTPROC glFramebufferTexture2DMultisampleEXT =
	//	(PFNGLFRAMEBUFFERTEXTURE2DMULTISAMPLEEXTPROC)eglGetProcAddress("glFramebufferTexture2DMultisampleEXT");

	for (int i = 0; i < frameBuffer->TextureSwapChainLength; i++)
	{
		// Create the color buffer texture.
		const GLuint colorTexture = vrapi_GetTextureSwapChainHandle(frameBuffer->ColorTextureSwapChain, i);
		GL(glBindTexture(GL_TEXTURE_2D, colorTexture));
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
		//	GL(glFramebufferTexture2DMultisampleEXT(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0, multisamples));
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
			GL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0));
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

static void ovrFramebuffer_Destroy(ovrFramebuffer * frameBuffer)
{
	GL(glDeleteFramebuffers(frameBuffer->TextureSwapChainLength, frameBuffer->FrameBuffers));
	GL(glDeleteRenderbuffers(frameBuffer->TextureSwapChainLength, frameBuffer->DepthBuffers));
	vrapi_DestroyTextureSwapChain(frameBuffer->ColorTextureSwapChain);

	free(frameBuffer->DepthBuffers);
	free(frameBuffer->FrameBuffers);

	ovrFramebuffer_Clear(frameBuffer);
}

static void ovrFramebuffer_SetCurrent(ovrFramebuffer * frameBuffer)
{
	GL(glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer->FrameBuffers[frameBuffer->TextureSwapChainIndex]));
}

static void ovrFramebuffer_SetNone()
{
	GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

static void ovrFramebuffer_Resolve(ovrFramebuffer * frameBuffer)
{
	// Discard the depth buffer, so the tiler won't need to write it back out to memory.
	//const GLenum depthAttachment[1] = { GL_DEPTH_ATTACHMENT };
	//glInvalidateFramebuffer(GL_FRAMEBUFFER, 1, depthAttachment);

	// Flush this frame worth of commands.
	glFlush();
}

static void ovrFramebuffer_Advance(ovrFramebuffer * frameBuffer)
{
	// Advance to the next texture from the set.
	frameBuffer->TextureSwapChainIndex = (frameBuffer->TextureSwapChainIndex + 1) % frameBuffer->TextureSwapChainLength;
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

	ovrFramebuffer  _frameBuffer[EYE_NUM];
	ovrMobile      *_ovr;
	ovrJava         _java;
	long long       _frameIndex;
	ovrMatrix4f     _projectionMatrix;
	ovrTracking     _tracking;
	ovrFrameParms   _frameParams;


	cocos2d::CustomCommand _beginRenderCommand;
	cocos2d::CustomCommand _endRenderCommand;
};

#endif // defined(GEAR_VR)

#endif
