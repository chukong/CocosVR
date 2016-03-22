#include "Tiger.h"

USING_NS_CC;
Tiger::~Tiger()
{

}

bool Tiger::init()
{
	bool state = Tank::init();
	_gunfire->setRotation3D(Vec3(0.0f, 0.0f, 0.0f));
	_gunfire->setScale(1.0f);
	_trackTextures[0] = TextureCache::getInstance()->addImage("models/tanks/Tiger/Tiger_Track2.dds");
	_trackTextures[1] = TextureCache::getInstance()->addImage("models/tanks/Tiger/Tiger_Track3.dds");
	_trackTextures[2] = TextureCache::getInstance()->addImage("models/tanks/Tiger/Tiger_Track4.dds");
	_trackTextures[3] = TextureCache::getInstance()->addImage("models/tanks/Tiger/Tiger_Track1.dds");
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

void Tiger::update(float delta)
{
	_latestShootTime += delta;
	if (_needUpdateCannonStage) {
		_cannon->setRotation3D(Vec3(0.0f, _cannonStageAngle, 0.0f));
		_needUpdateCannonStage = false;
	}

	if (_needUpdateGunAngle) {
		auto gun = _cannon->getChildByName("cannon");
		gun->setRotation3D(Vec3(-_cannonGunAngle, 0.0f, 0.0f));
		_needUpdateGunAngle = false;
	}

	if (0.0f < _pointLight->getIntensity()) {
		float intensity = _pointLight->getIntensity() - 5.0f * delta;
		_pointLight->setIntensity(0.0f <= intensity ? intensity : 0.0f);
	}
}

Tiger::Tiger()
{

}

std::string Tiger::getModelFilePath()
{
	return "models/tanks/Tiger/Tiger.c3b";
}

std::string Tiger::getDamagedModelFilePath()
{
	return "models/tanks/Tiger/Tiger_damaged.c3b";
}
