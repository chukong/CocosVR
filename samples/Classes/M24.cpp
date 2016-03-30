#include "M24.h"

USING_NS_CC;
M24::~M24()
{

}

bool M24::init()
{
	bool state = Tank::init();
	_trackTextures[0] = TextureCache::getInstance()->addImage("models/tanks/M24/M24_Track2.dds");
	_trackTextures[1] = TextureCache::getInstance()->addImage("models/tanks/M24/M24_Track3.dds");
	_trackTextures[2] = TextureCache::getInstance()->addImage("models/tanks/M24/M24_Track4.dds");
	_trackTextures[3] = TextureCache::getInstance()->addImage("models/tanks/M24/M24_Track1.dds");
	Texture2D::TexParams texParams;
	texParams.minFilter = GL_LINEAR;
	texParams.magFilter = GL_LINEAR;
	texParams.wrapS = GL_REPEAT;
	texParams.wrapT = GL_REPEAT;
	_trackTextures[0]->setTexParameters(texParams);
	_trackTextures[1]->setTexParameters(texParams);
	_trackTextures[2]->setTexParameters(texParams);
	_trackTextures[3]->setTexParameters(texParams);
	return state;
}

M24::M24()
{

}

std::string M24::getModelFilePath()
{
	return "models/tanks/M24/M24.c3b";
}

std::string M24::getDamagedModelFilePath()
{
	return "models/tanks/M24/M24_damaged.c3b";
}
