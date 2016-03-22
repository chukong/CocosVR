#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "physics3d/CCPhysics3D.h"
#include "ProgressView .h"

class Tank;
class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
	virtual void update(float delta) override;
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

private:

	struct TankState
	{
		Tank *tank;
		ProgressView *pv;
		float moveTime;
		float randomAngle;
		bool needChangeDir;
	};

	void generateEnemy(const cocos2d::Vec3 &pos, const cocos2d::Color3B &col, unsigned short type);

	void playerUpdate(float delta);
	void enemyAI(float delta);
	void enemyWalking(TankState &enemy, float delta);
	void enemyTracking(TankState &enemy, float delta);
	void enemyEscaping(TankState &enemy, float delta);
	void removeEnemy(Tank *enemy);

private:

	std::vector<TankState> _enemyList;
	Tank *_player;
	cocos2d::Physics3DObject *_terrainCollider;
};

#endif // __HELLOWORLD_SCENE_H__
