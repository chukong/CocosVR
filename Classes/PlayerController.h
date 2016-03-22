#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__
#include "cocos2d.h"
#include "base/CCController.h"

class OVRRenderer;
class Tank;

enum class PlayerMoveState
{
	FRONT,
	BACK,
	STOP,
};

enum class PlayerRotateState
{
	TURN,
	STOP,
};

enum class CannonState
{
	ROTATE_LEFT,
	ROTATE_RIGHT,
	STOP,
};

enum class GunState
{
	GUN_UP,
	GUN_DOWN,
	STOP,
};

class PlayerController : public cocos2d::Layer
{
public:
	~PlayerController();

	static PlayerController* create();

	virtual void update(float delta) override;
	void setOVRRenderer(OVRRenderer *renderer) { _ovrRenderer = renderer; }
	void setPlayer(Tank *tank) { _player = tank; }

private:
	PlayerController();

private:

	bool init();
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event *event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event *event);

	void onControllerConnected(cocos2d::Controller *controller, cocos2d::Event *event);
	void onControllerDisconnected(cocos2d::Controller *controller, cocos2d::Event *event);
	void onControllerKeyPressed(cocos2d::Controller *controller, int key, cocos2d::Event *event);
	void onControllerKeyReleased(cocos2d::Controller *controller, int key, cocos2d::Event *event);
	void onControllerAxisEvent(cocos2d::Controller *controller, int key, cocos2d::Event *event);

private:

	OVRRenderer *_ovrRenderer;
	Tank        *_player;
	PlayerMoveState   _moveState;
	PlayerRotateState _rotateState;
	CannonState       _cannonState;
	GunState          _gunState;

	unsigned int      _pressNum;
	cocos2d::Vec3     _moveDir;
	std::vector<cocos2d::EventKeyboard::KeyCode> _keyCodeStack;
};

#endif