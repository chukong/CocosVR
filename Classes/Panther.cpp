#include "Panther.h"

USING_NS_CC;
Panther::~Panther()
{

}

bool Panther::init()
{
	bool state = Tank::init();
	_trackTextures[0] = TextureCache::getInstance()->addImage("models/tanks/Panther/Panther_Track2.dds");
	_trackTextures[1] = TextureCache::getInstance()->addImage("models/tanks/Panther/Panther_Track3.dds");
	_trackTextures[2] = TextureCache::getInstance()->addImage("models/tanks/Panther/Panther_Track4.dds");
	_trackTextures[3] = TextureCache::getInstance()->addImage("models/tanks/Panther/Panther_Track1.dds");
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

Panther::Panther()
{

}

std::string Panther::getModelFilePath()
{
	return "models/tanks/Panther/Panther.c3b";
}

std::string Panther::getDamagedModelFilePath()
{
	return "models/tanks/Panther/Panther_damaged.c3b";
}
