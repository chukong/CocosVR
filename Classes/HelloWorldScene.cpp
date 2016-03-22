#include "HelloWorldScene.h"
#include "Tank.h"
#include "M24.h"
#include "P4F2.h"
#include "Panther.h"
#include "Tiger.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#include "OVRRenderer.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "OVRRenderer-android.h"
#endif
#include "PlayerController.h"
#include "physics3d/CCPhysics3D.h"
#include "3d/CCBundle3D.h"
#include "SimpleAudioEngine.h"
#include "navmesh/CCNavMesh.h"

USING_NS_CC;
using namespace CocosDenshion;

#define HP_REDUCE_VALUE 20
#define HP_LOWER_LIMIT  50

#define SMOKE_PS_TAG  0x0F
#define TARGET_TAG    0xFF

#define WALKING   1
#define TRACKING  2
#define ESCAPING  3
#define DEAD      4

void recycleSetTextureParams(Node *node, const Texture2D::TexParams &params)
{
	if (dynamic_cast<Sprite3D *>(node)) {
		auto sprite3d = dynamic_cast<Sprite3D *>(node);
		for (unsigned int i = 0; i < sprite3d->getMeshCount(); ++i) {
			auto mesh = sprite3d->getMeshByIndex(i);
			mesh->getTexture()->setTexParameters(params);
			mesh->getTexture()->generateMipmap();
		}
	}
	for (auto &child : node->getChildren()) {
		recycleSetTextureParams(child, params);
	}
}

//unsigned short ENEMY_STATE = WALKING;

Scene* HelloWorld::createScene()
{
    // 'layer' is an autorelease object
    auto scene = HelloWorld::create();
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

	if (!initWithPhysics())
	{
		return false;
	}

	//getPhysics3DWorld()->setDebugDrawEnable(true);


    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

	//{
	//	auto sprite3d = Sprite3D::create("models/tank/tank.c3b");
	//	sprite3d->setScale(1.0f);
	//	sprite3d->setRotation3D(Vec3(0.0f, 0.0f, 0.0f));
	//	sprite3d->setCameraMask((unsigned short)CameraFlag::USER1);
	//	this->addChild(sprite3d);
	//}

	//std::string fileName = "models/tanks/P4F2/P4F2.c3b";
	//std::string texName = "models/tanks/P4F2/P4F2_Track";

	//auto sprite3d = Sprite3D::create(fileName);
	//sprite3d->setScale(0.03f);
	//sprite3d->setRotation3D(Vec3(0.0f, 0.0f, 0.0f));
	//sprite3d->setCameraMask((unsigned short)CameraFlag::USER1);
	//this->addChild(sprite3d);

	//auto tex0 = TextureCache::getInstance()->addImage(texName + std::string("2.dds"));
	//auto tex1 = TextureCache::getInstance()->addImage(texName + std::string("3.dds"));
	//auto tex2 = TextureCache::getInstance()->addImage(texName + std::string("4.dds"));
	//auto tex3 = TextureCache::getInstance()->addImage(texName + std::string("1.dds"));
	//Texture2D::TexParams texParams;
	//texParams.minFilter = GL_LINEAR;
	//texParams.magFilter = GL_LINEAR;
	//texParams.wrapS = GL_REPEAT;
	//texParams.wrapT = GL_REPEAT;
	//tex0->setTexParameters(texParams);
	//tex1->setTexParameters(texParams);
	//tex2->setTexParameters(texParams);
	//tex3->setTexParameters(texParams);

	//sprite3d->getChildByName("turret")->setRotation3D(Vec3(-90.0f, 90.0f, 0.0f));
	//sprite3d->runAction(RepeatForever::create(Sequence::create(CallFunc::create([sprite3d, texName]() {
	//	static Texture2D *trackTexs[4] = { TextureCache::getInstance()->getTextureForKey(texName + std::string("2.dds"))
	//	, TextureCache::getInstance()->getTextureForKey(texName + std::string("3.dds"))
	//	, TextureCache::getInstance()->getTextureForKey(texName + std::string("4.dds"))
	//	, TextureCache::getInstance()->getTextureForKey(texName + std::string("1.dds")) };
	//	static float speed = 0.0f;
	//	static unsigned short texIndex = 0;
	//	if (0.05 < speed) {
	//		static_cast<Sprite3D *>(sprite3d->getChildByName("Lefttrack"))->setTexture(trackTexs[texIndex]);
	//		static_cast<Sprite3D *>(sprite3d->getChildByName("Righttrack"))->setTexture(trackTexs[texIndex]);
	//		speed = 0.0f;
	//		texIndex = (texIndex + 1) % 4;
	//	}
	//	speed += Director::getInstance()->getDeltaTime();
	//}), nullptr)));

	//{
	//	auto camera = Camera::createPerspective(30.0f, visibleSize.width / visibleSize.height, 1.0f, 10000.0f);
	//	camera->setPosition3D(Vec3(0.0f, 0.0f, 10.0f));
	//	camera->lookAt(Vec3::ZERO);
	//	camera->setCameraFlag(CameraFlag::USER1);
	//	this->addChild(camera);
	//}

	//{
	//	auto camera = Camera::createPerspective(30.0f, visibleSize.width / visibleSize.height, 1.0f, 5000.0f);
	//	camera->setPosition3D(Vec3(0.0, 400.0f, 400.0f));
	//	camera->lookAt(Vec3(0.0f, 0.0f, 0.0f));
	//	camera->setCameraFlag(CameraFlag::USER1);
	//	this->addChild(camera);
	//	this->setPhysics3DDebugCamera(camera);
	//}

	auto navMesh = NavMesh::create("NavMesh/all_tiles_tilecache.bin", "NavMesh/geomset.txt");
	navMesh->setDebugDrawEnable(false);
	setNavMesh(navMesh);

	auto pc = PlayerController::create();
	this->addChild(pc);

	_player = M24::create();
	_player->setPosition3D(Vec3(0.0f, 0.0f, 120.0f));
	_player->setCameraMask((unsigned short)CameraFlag::USER1);
	this->addChild(_player);

	generateEnemy(Vec3(0.0f, 0.0f, -120.0f), Color3B::RED, 0);
	generateEnemy(Vec3(120.0f, 0.0f, 0.0f), Color3B::GREEN, 1);
	generateEnemy(Vec3(-120.0f, 0.0f, 0.0f), Color3B::BLUE, 2);

	auto ovrRenderer = OVRRenderer::create(CameraFlag::USER1);
	_player->addChild(ovrRenderer);
	ovrRenderer->setOffsetPos(Vec3(0.0f, 3.2f, 0.0f));

	pc->setPlayer(_player);
	pc->setOVRRenderer(ovrRenderer);

	auto skybox = Sprite3D::create("effects/skybox/skybox.c3t");
	skybox->setCameraMask((unsigned short)CameraFlag::USER1);
	skybox->setScale(1000.0f);
	this->addChild(skybox);

	Texture2D::TexParams texParams;
	texParams.minFilter = GL_LINEAR_MIPMAP_LINEAR;
	texParams.magFilter = GL_LINEAR;
	texParams.wrapS = GL_REPEAT;
	texParams.wrapT = GL_REPEAT;

	float scale = 1.0f;
	std::vector<Vec3> trianglesList = Bundle3D::getTrianglesList("models/terrain/terrain.obj");
	for (auto& it : trianglesList) {
		it *= scale;
		//it -= Vec3(0.0f, 0.0f, 4.0f);
	}

	Physics3DRigidBodyDes rbDes;
	rbDes.mass = 0.0f;
	rbDes.shape = Physics3DShape::createMesh(&trianglesList[0], (int)trianglesList.size() / 3);
	_terrainCollider = Physics3DRigidBody::create(&rbDes);
	auto component = Physics3DComponent::create(_terrainCollider);
	auto terrain = Sprite3D::create("models/terrain/terrain.obj");
	terrain->addComponent(component);
	//terrain->setRotation3D(Vec3(-90.0f, 0.0f, 0.0f));
	//terrain->setPosition3D(Vec3(0.0f, 2.0f, 0.0f));
	terrain->setScale(scale);
	terrain->setCameraMask((unsigned short)CameraFlag::USER1);
	recycleSetTextureParams(terrain, texParams);
	this->addChild(terrain);


	_player->setAttackCallback([=](const Physics3DCollisionInfo &ci) {
		if (ci.objB == _terrainCollider) {
			PUParticleSystem3D *ps = PUParticleSystem3D::create("effects/Particle3D/scripts/dirtExplosion.pu");
			ps->runAction(Sequence::create(DelayTime::create(5.0f), CallFunc::create([ps]() {
				ps->removeFromParent();
			}), nullptr));
			ps->setPosition3D(ci.collisionPointList[0].worldPositionOnB);
			ps->startParticleSystem();
			ps->setCameraMask((unsigned short)CameraFlag::USER1);
			this->addChild(ps);
			SimpleAudioEngine::getInstance()->playEffect(FileUtils::getInstance()->fullPathForFilename("sound/explosion1.mp3").c_str());
		}
		else {
			PUParticleSystem3D *ps = PUParticleSystem3D::create("effects/Particle3D/scripts/metalExplosion.pu");
			ps->setScale(0.5f);
			ps->runAction(Sequence::create(DelayTime::create(3.0f), CallFunc::create([ps]() {
				ps->removeFromParent();
			}), nullptr));
			ps->setPosition3D(ci.collisionPointList[0].worldPositionOnB);
			ps->startParticleSystem();
			ps->setCameraMask((unsigned short)CameraFlag::USER1);
			this->addChild(ps);

			auto enemy = static_cast<Tank *>(ci.objB->getUserData());

			enemy->setHP(enemy->getHP() - HP_REDUCE_VALUE);			

			for (auto &e : _enemyList) {
				if (e.tank == enemy) {
					e.pv->setCurrentProgress(e.pv->getCurrentProgress() - HP_REDUCE_VALUE);
				}
			}
			//if (_enemy->getHP() <= HP_LOWER_LIMIT) {
			//	if (!_enemy->getChildByTag(SMOKE_PS_TAG)) {
			//		auto smoke = PUParticleSystem3D::create("effects/Particle3D/scripts/smoke.pu");
			//		smoke->setScale(0.05f);
			//		smoke->startParticleSystem();
			//		smoke->setCameraMask(_enemy->getCameraMask());
			//		_enemy->addChild(smoke, 0, SMOKE_PS_TAG);

			//		_enemy->setTexture(_crashTexture);
			//	}
			//}

			if (enemy->getHP() == 0.0f) {
				auto explosion = PUParticleSystem3D::create("effects/Particle3D/scripts/explosionSystem.pu");
				explosion->setScale(5.0f);
				explosion->startParticleSystem();
				explosion->setCameraMask(enemy->getCameraMask());
				explosion->setPosition3D(enemy->getPosition3D());
				this->addChild(explosion);
				explosion->runAction(Sequence::create(DelayTime::create(5.0f), CallFunc::create([explosion]() {
					explosion->removeFromParent();
				}), nullptr));

				//this->runAction(Sequence::create(DelayTime::create(6.0f), CallFunc::create([this]() {
				//	this->generateEnemy();
				//	this->scheduleUpdate();
				//}), nullptr));
				enemy->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([this, enemy]() {
					auto enemy_damaged = Sprite3D::create(enemy->getDamagedModelFilePath());
					enemy_damaged->setCameraMask(enemy->getCameraMask());
					enemy_damaged->setPosition3D(enemy->getPosition3D());
					enemy_damaged->setRotationQuat(enemy->getRotationQuat());
					enemy_damaged->setScale(enemy->getScale());
					this->addChild(enemy_damaged);
					enemy_damaged->runAction(Sequence::create(CallFunc::create([enemy_damaged]() {
						enemy_damaged->runAction(MoveBy::create(5.0f, Vec3(0.0f, -5.0f, 0.0f)));
					}), DelayTime::create(5.0f), CallFunc::create([enemy_damaged]() {
						enemy_damaged->removeFromParent();
					}), nullptr));
					this->removeEnemy(enemy);
				}), nullptr));

				SimpleAudioEngine::getInstance()->playEffect(FileUtils::getInstance()->fullPathForFilename("sound/boom.mp3").c_str());
			}

			SimpleAudioEngine::getInstance()->playEffect(FileUtils::getInstance()->fullPathForFilename("sound/metal_crash1.mp3").c_str());
		}

		//CCLOG("(%f, %f, %f)", ci.collisionPointList[0].worldPositionOnB.x, ci.collisionPointList[0].worldPositionOnB.y, ci.collisionPointList[0].worldPositionOnB.z);
		for (auto &enemy : _enemyList) {
			if ((ci.collisionPointList[0].worldPositionOnB - enemy.tank->getPosition3D()).length() < 10.0f) {
				enemy.tank->setState(TRACKING);
			}
		}
	});

	SimpleAudioEngine::getInstance()->playBackgroundMusic(FileUtils::getInstance()->fullPathForFilename("sound/background.mp3").c_str(), true);

	auto ambient = AmbientLight::create(Color3B::WHITE);
	this->addChild(ambient);

	scheduleUpdate();
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::update(float delta)
{
	enemyAI(delta);
}

void HelloWorld::enemyAI(float delta)
{
	for (auto &enemy : _enemyList) {
		//Vec3 safeDis = _player->getPosition3D() - enemy.tank->getPosition3D();
		//if (safeDis.length() < 50.0f) {
		//	enemy.tank->getChildByTag(TARGET_TAG)->setVisible(false);
		//}
		//else {
		//	enemy.tank->getChildByTag(TARGET_TAG)->setVisible(true);
		//}

		switch (enemy.tank->getState())
		{
		case WALKING:
			enemyWalking(enemy, delta);
			break;

		case TRACKING:
			enemyTracking(enemy, delta);
			break;

		case ESCAPING:
			enemyEscaping(enemy, delta);
			break;

		default:
			break;
		}
	}
}

void HelloWorld::enemyWalking(TankState &enemy, float delta)
{
	if (enemy.tank->getHP() <= 0.0f) return;

	//enemy.moveTime -= delta;
	//if (enemy.moveTime <= 0.0f) {
	//	enemy.moveTime = CCRANDOM_0_1() * 50.0f + 50.0f;
	//	enemy.needChangeDir = true;
	//}

	//if (enemy.needChangeDir) {
	//	enemy.randomAngle = 2.0;
	//	enemy.needChangeDir = false;
	//}

	//if (0.0f < abs(enemy.randomAngle)) {
	//	enemy.tank->turn(enemy.randomAngle);
	//	float latestAngle = enemy.randomAngle;
	//	enemy.randomAngle -= delta * enemy.randomAngle;
	//	if (latestAngle * enemy.randomAngle < 0.0)
	//		enemy.randomAngle = 0.0f;
	//}

	//if (!enemy.tank->move(delta * 3.0f)) {
	//	enemy.needChangeDir = true;
	//}
	static float changeDirTime = 0.0f;
	if (changeDirTime <= 0.0f) {
		changeDirTime = CCRANDOM_0_1() * 7.0f + 3.0f;
		enemy.tank->move(Vec3(CCRANDOM_0_1(), 0.0f, CCRANDOM_0_1()) * 130.0f);
	}
	else {
		changeDirTime -= delta;
	}

	Vec3 safeDis = _player->getPosition3D() - enemy.tank->getPosition3D();
	if (safeDis.length() < 50.0f) {
		enemy.tank->setState(TRACKING);
	}
}

void HelloWorld::enemyTracking(TankState &enemy, float delta)
{
	if (enemy.tank->getHP() <= 0.0f) return;

	Vec3 dis = _player->getPosition3D() - enemy.tank->getPosition3D();
	Vec3 deltaDir = dis;
	deltaDir.normalize();

	//Vec3 enemydir = enemy.tank->getRotationQuat() * -Vec3::UNIT_Z;
	//enemydir.normalize();

	//float theta = acos(clampf(enemydir.dot(deltaDir), 0.0f, 1.0f));
	//Vec3 up;
	//Vec3::cross(enemydir, deltaDir, &up);
	//if (up.y < 0.0)
	//	theta = -theta;

	//enemy.tank->turn(delta * CC_RADIANS_TO_DEGREES(theta));
	//enemy.tank->move(delta * 3.0f);

	enemy.tank->move(_player->getPosition3D());

	if (enemy.tank->shot(_player->getPosition3D(), 100.0f + CCRANDOM_0_1() * (15.0f * (100.0f / dis.length())))) {
		SimpleAudioEngine::getInstance()->playEffect(FileUtils::getInstance()->fullPathForFilename("sound/enemy_shot.mp3").c_str());
	}

	Vec3 safeDis = _player->getPosition3D() - enemy.tank->getPosition3D();
	if (100.0f < safeDis.length()) {
		enemy.tank->setState(WALKING);
	}
}

void HelloWorld::enemyEscaping(TankState &enemy, float delta)
{
	if (enemy.tank->getHP() <= 0.0f) return;

	Vec3 deltaDir = enemy.tank->getPosition3D() - _player->getPosition3D();
	deltaDir.normalize();

	Vec3 enemydir = enemy.tank->getRotationQuat() * -Vec3::UNIT_Z;
	enemydir.normalize();

	float theta = acos(clampf(enemydir.dot(deltaDir), 0.0f, 1.0f));
	Vec3 up;
	Vec3::cross(enemydir, deltaDir, &up);
	if (up.y < 0.0)
		theta = -theta;

	static float moveSpeed = 3.0f;
	if (enemy.tank->getHP() <= 50.0f && 30.0 < enemy.tank->getHP()) {
		moveSpeed = 2.0f;
	}
	else if (enemy.tank->getHP() <= 30.0f) {
		moveSpeed = 1.0f;
	}
	enemy.tank->turn(delta * CC_RADIANS_TO_DEGREES(theta));
	enemy.tank->move(delta * moveSpeed);

	if (enemy.tank->shot(_player->getPosition3D(), 100.0f + CCRANDOM_0_1() * 15.0f)) {
		SimpleAudioEngine::getInstance()->playEffect(FileUtils::getInstance()->fullPathForFilename("sound/enemy_shot.mp3").c_str());
	}
}

void HelloWorld::playerUpdate(float delta)
{
	if (_player->getHP() <= 0.0) {
		CCLOG("GAME OVER!");
	}
}

void HelloWorld::generateEnemy(const cocos2d::Vec3 &pos, const Color3B &col, unsigned short type)
{
	Tank *enemy = nullptr;
	if (type == 0) {
		enemy = P4F2::create();
	}
	else if (type == 1) {
		enemy = Panther::create();
	}
	else if (type == 2) {
		enemy = Tiger::create();
	}
	if (!enemy) return;
	//_enemy->setPosition3D(Vec3(0.0f, -50.0f, -120.0f));
	//_enemy->setRotation3D(Vec3(0.0f, 180.0f, 0.0f));
	enemy->setPosition3D(pos);
	enemy->setRotation3D(Vec3(0.0f, 180.0f, 0.0f));
	enemy->retain();
	this->addChild(enemy);


	//auto sprite = BillBoard::create("target.png");
	//sprite->setCameraMask((unsigned short)CameraFlag::USER1);
	//sprite->setScale(0.05f);
	//sprite->setColor(col);
	//sprite->setPosition3D(Vec3(0.0f, 10.0f, 0.0f));
	//enemy->addChild(sprite, 0, TARGET_TAG);

	auto bb = BillBoard::create();
	auto pv = ProgressView::create();
	bb->setScale(0.1f);
	bb->setPosition3D(Vec3(0.0f, 5.0f, 0.0f));
	bb->setCameraMask((unsigned short)CameraFlag::USER1);
	bb->addChild(pv);
	pv->setForegroundTexture("blood.png");
	pv->setTotalProgress(100.0f);
	//pv->setForegroundColor(col);
	enemy->addChild(bb);


	enemy->setAttackCallback([=](const Physics3DCollisionInfo &ci) {
		if (ci.objB == _terrainCollider) {
			PUParticleSystem3D *ps = PUParticleSystem3D::create("effects/Particle3D/scripts/dirtExplosion.pu");
			ps->runAction(Sequence::create(DelayTime::create(5.0f), CallFunc::create([ps]() {
				ps->removeFromParent();
			}), nullptr));
			ps->setPosition3D(ci.collisionPointList[0].worldPositionOnB);
			ps->startParticleSystem();
			ps->setCameraMask((unsigned short)CameraFlag::USER1);
			this->addChild(ps);
			SimpleAudioEngine::getInstance()->playEffect(FileUtils::getInstance()->fullPathForFilename("sound/explosion2.mp3").c_str());
		}
		else {
			PUParticleSystem3D *ps = PUParticleSystem3D::create("effects/Particle3D/scripts/metalExplosion.pu");
			ps->setScale(0.5f);
			ps->runAction(Sequence::create(DelayTime::create(3.0f), CallFunc::create([ps]() {
				ps->removeFromParent();
			}), nullptr));
			ps->setPosition3D(ci.collisionPointList[0].worldPositionOnB);
			ps->startParticleSystem();
			ps->setCameraMask((unsigned short)CameraFlag::USER1);
			this->addChild(ps);
			SimpleAudioEngine::getInstance()->playEffect(FileUtils::getInstance()->fullPathForFilename("sound/metal_crash2.mp3").c_str());
			//if (ci.objB->getUserData()) {
			//	Tank *tank = static_cast<Tank *>(ci.objB->getUserData());
			//	tank->setHP(tank->getHP() - HP_REDUCE_VALUE);
			//}
		}
	});

	enemy->setCameraMask((unsigned short)CameraFlag::USER1);
	enemy->setState(WALKING);
	_enemyList.push_back({enemy, pv, 0.0f, 0.0f, false});
}

void HelloWorld::removeEnemy(Tank *enemy)
{
	enemy->removeFromParent();
	for (std::vector<TankState>::iterator iter = _enemyList.begin(); iter != _enemyList.end(); ++iter) {
		if (iter->tank == enemy) {
			_enemyList.erase(iter);
			return;
		}
	}
}
