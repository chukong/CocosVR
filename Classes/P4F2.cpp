#include "P4F2.h"

USING_NS_CC;
P4F2::~P4F2()
{

}

bool P4F2::init()
{
	bool state = Tank::init();
	_trackTextures[0] = TextureCache::getInstance()->addImage("models/tanks/P4F2/P4F2_Track2.dds");
	_trackTextures[1] = TextureCache::getInstance()->addImage("models/tanks/P4F2/P4F2_Track3.dds");
	_trackTextures[2] = TextureCache::getInstance()->addImage("models/tanks/P4F2/P4F2_Track4.dds");
	_trackTextures[3] = TextureCache::getInstance()->addImage("models/tanks/P4F2/P4F2_Track1.dds");
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

P4F2::P4F2()
{

}

std::string P4F2::getModelFilePath()
{
	return "models/tanks/P4F2/P4F2.c3b";
}

std::string P4F2::getDamagedModelFilePath()
{
	return "models/tanks/P4F2/P4F2_damaged.c3b";
}
