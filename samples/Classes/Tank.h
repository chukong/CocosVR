#ifndef __TANK_H__
#define __TANK_H__

#include "cocos2d.h"
#include "extensions/Particle3D/PU/CCPUParticleSystem3D.h"
#include "physics3d/CCPhysics3D.h"
#include "navmesh/CCNavMesh.h"

class Tank : public cocos2d::Sprite3D
{
public:

	virtual bool init();
	virtual void update(float delta) override;

	bool move(const cocos2d::Vec3 &target);
	bool move(float force);
	void turn(float torque);
	bool shot(float speed);
	bool shot(const cocos2d::Vec3 &target, float speed);
	void rotateCannonStage(float angle);
	void rotateCannonGun(float angle);
	void setTexture(cocos2d::Texture2D *texture);
	float getCannonStageAngle() const { return _cannonStageAngle - 180.0f; }
	float getCannonGunAngle() const { return _cannonGunAngle; }
	float getTurnAngle() const { return _turnAngle; }
	cocos2d::Node *getCannonStageNode() const;
	
	void setAttackCallback(const cocos2d::Physics3DObject::CollisionCallbackFunc &func) { _callBack = func; }

	void setHP(float hp);
	float getHP() const { return _hp; }

	void setState(unsigned short state) { _state = state; }
	unsigned short getState() const { return _state; }

	virtual std::string getModelFilePath() = 0;
	virtual std::string getDamagedModelFilePath() = 0;

protected:

	Tank();
	void shotBullet(float speed);
	void changeLeftTrackTexture(float step);
	void changeRightTrackTexture(float step);

	bool initWithFile(const std::string &path);
	bool initFrom(const cocos2d::NodeDatas& nodedatas, const cocos2d::MeshDatas& meshdatas, const cocos2d::MaterialDatas& materialdatas);
	void createNode(cocos2d::NodeData* nodedata, cocos2d::Node* root, const cocos2d::MaterialDatas& matrialdatas, bool singleSprite);

protected:

	cocos2d::Vec3 _moveTarget;
	cocos2d::Node *_cannon;
	float _turnAngle;
	float _cannonStageAngle;
	float _cannonGunAngle;
	float _hp;
	float _latestShootTime;
	bool  _needUpdateCannonStage;
	bool  _needUpdateGunAngle;
	unsigned short _state;

	bool  _shotDampingMotion;
	float _shotDampingTime;

	cocos2d::PUParticleSystem3D *_gunfire;
	cocos2d::Physics3DObject::CollisionCallbackFunc _callBack;
	cocos2d::PointLight *_pointLight;
	cocos2d::Texture2D  *_trackTextures[4];

	cocos2d::NavMeshAgent *_agent;
};

#endif // __HELLOWORLD_SCENE_H__
