#include "PlayerController.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#include "OVRRenderer.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "OVRRenderer-android.h"
#endif
#include "Tank.h"
#include "SimpleAudioEngine.h"
#include "base/CCEventListenerController.h"

USING_NS_CC;
using namespace CocosDenshion;

#define BUTTON_L2 1017
#define BUTTON_R2 1018

#define TANK_ROTATE_SPEED     2.0f
#define TANK_MAX_ROTATE_SPEED 1.0f
#define CANNON_STAGE_ROTATE_SPEED     2.0f
#define CANNON_STAGE_MAX_ROTATE_SPEED 1.0f
#define CANNON_GUN_ROTATE_SPEED       3.0f
#define CANNON_GUN_MAX_ROTATE_SPEED   0.5f
#define TANK_MOVE_SPEED           0.3f
#define TANK_MAX_MOVE_SPEED       0.5f

//#define TANK_MOVE_FORCE           5.0f
//#define TANK_TURN_TORQUE          45.0f
#define TANK_BULLET_SPEED         100.0f

static bool  CONNON_ROTATE_ON = false;
static float CONNON_ROTATE_TIME = 0.0f;

static bool  GUN_ROTATE_ON = false;
static float GUN_ROTATE_TIME = 0.0f;

static bool  TANK_ROTATE_ON = false;
static float TANK_ROTATE_TIME = 0.0f;

static bool  TANK_MOVE_ON = false;
static float TANK_MOVE_TIME = 0.0f;

static unsigned int CONNON_ROTATE_SOUND = 0;
static unsigned int TANK_MOVE_SOUND = 0;

PlayerController::PlayerController()
	: _ovrRenderer(nullptr)
	, _player(nullptr)
	, _moveState(PlayerMoveState::STOP)
	, _rotateState(PlayerRotateState::STOP)
	, _cannonState(CannonState::STOP)
	, _gunState(GunState::STOP)
	, _pressNum(0)
{
}

PlayerController::~PlayerController()
{

}

void PlayerController::onKeyPressed(EventKeyboard::KeyCode code, Event *event)
{
	if (code == EventKeyboard::KeyCode::KEY_LEFT_ARROW) {
		if (_cannonState == CannonState::ROTATE_LEFT) 
			return;
		_cannonState = CannonState::ROTATE_LEFT;
		CONNON_ROTATE_TIME = 0.0f;
		CONNON_ROTATE_ON = true;
		CONNON_ROTATE_SOUND = SimpleAudioEngine::getInstance()->playEffect(FileUtils::getInstance()->fullPathForFilename("sound/cannon_rotate2.mp3").c_str(), true);
	}
	else if (code == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) {
		if (_cannonState == CannonState::ROTATE_RIGHT)
			return;
		_cannonState = CannonState::ROTATE_RIGHT;
		CONNON_ROTATE_TIME = 0.0f;
		CONNON_ROTATE_ON = true;
		CONNON_ROTATE_SOUND = SimpleAudioEngine::getInstance()->playEffect(FileUtils::getInstance()->fullPathForFilename("sound/cannon_rotate2.mp3").c_str(), true);
	}

	if (code == EventKeyboard::KeyCode::KEY_UP_ARROW) {
		if (_gunState == GunState::GUN_UP)
			return;
		_gunState = GunState::GUN_UP;
		GUN_ROTATE_TIME = 0.0f;
		GUN_ROTATE_ON = true;
	}
	else if (code == EventKeyboard::KeyCode::KEY_DOWN_ARROW) {
		if (_gunState == GunState::GUN_DOWN)
			return;
		_gunState = GunState::GUN_DOWN;
		GUN_ROTATE_TIME = 0.0f;
		GUN_ROTATE_ON = true;
	}

	if (code == EventKeyboard::KeyCode::KEY_SPACE || code == EventKeyboard::KeyCode::KEY_DPAD_CENTER) {
		//_cannonState = CannonState::SHOT;
		if (_player->shot(TANK_BULLET_SPEED)) {
			SimpleAudioEngine::getInstance()->playEffect(FileUtils::getInstance()->fullPathForFilename("sound/player_shot.mp3").c_str());
		}
	}

	if (code == EventKeyboard::KeyCode::KEY_A
		|| code == EventKeyboard::KeyCode::KEY_D
		|| code == EventKeyboard::KeyCode::KEY_W
		|| code == EventKeyboard::KeyCode::KEY_S) {
		if (_rotateState != PlayerRotateState::TURN) {
			_rotateState = PlayerRotateState::TURN;
			TANK_ROTATE_TIME = 0.0f;
			TANK_ROTATE_ON = true;
		}

		if (_moveState != PlayerMoveState::FRONT) {
			_moveState = PlayerMoveState::FRONT;
			TANK_MOVE_TIME = 0.0f;
			TANK_MOVE_ON = true;
		}

		if (code == EventKeyboard::KeyCode::KEY_A) {
			_moveDir.x = -1.0f;
		}
		else if (code == EventKeyboard::KeyCode::KEY_D) {
			_moveDir.x = 1.0f;
		}
		else if (code == EventKeyboard::KeyCode::KEY_W) {
			_moveDir.z = -1.0f;
		}
		else if (code == EventKeyboard::KeyCode::KEY_S) {
			_moveDir.z = 1.0f;
		}

		auto iter = std::find(_keyCodeStack.begin(), _keyCodeStack.end(), code);
		if (iter == _keyCodeStack.end()) {
			_keyCodeStack.push_back(code);
		}
	}
	//else if (code == EventKeyboard::KeyCode::KEY_D) {
	//	if (_rotateState == PlayerRotateState::TURN)
	//		return;
	//	_rotateState = PlayerRotateState::TURN;
	//	TANK_ROTATE_TIME = 0.0f;
	//	TANK_ROTATE_ON = true;
	//}
	//else if (code == EventKeyboard::KeyCode::KEY_W) {
	//	if (_moveState == PlayerMoveState::FRONT)
	//		return;
	//	_moveState = PlayerMoveState::FRONT;
	//	TANK_MOVE_TIME = 0.0f;
	//	TANK_MOVE_ON = true;
	//}
	//else if (code == EventKeyboard::KeyCode::KEY_S) {
	//	if (_moveState == PlayerMoveState::BACK)
	//		return;
	//	_moveState = PlayerMoveState::BACK;
	//	TANK_MOVE_TIME = 0.0f;
	//	TANK_MOVE_ON = true;
	//}

	CCLOG("onKeyPressed---: %d", (int)code);
}

void PlayerController::onKeyReleased(EventKeyboard::KeyCode code, Event *event)
{
	if (code == EventKeyboard::KeyCode::KEY_LEFT_ARROW
		|| code == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) {
		//_cannonState = CannonState::STOP;
		CONNON_ROTATE_ON = false;
	}

	if (code == EventKeyboard::KeyCode::KEY_UP_ARROW
		|| code == EventKeyboard::KeyCode::KEY_DOWN_ARROW) {
		//_gunState = GunState::STOP;
		GUN_ROTATE_ON = false;
	}

	if (code == EventKeyboard::KeyCode::KEY_A
		|| code == EventKeyboard::KeyCode::KEY_D
		|| code == EventKeyboard::KeyCode::KEY_W
		|| code == EventKeyboard::KeyCode::KEY_S
		) {
		auto iter = std::find(_keyCodeStack.begin(), _keyCodeStack.end(), code);
		_keyCodeStack.erase(iter);
		//_rotateState = PlayerRotateState::STOP;
		if (!_keyCodeStack.empty()) {
			onKeyPressed(_keyCodeStack.back(), nullptr);
		}
		else {
			_moveDir = Vec3::ZERO;
			TANK_ROTATE_ON = false;
			TANK_MOVE_ON = false;
		}

		if (code == EventKeyboard::KeyCode::KEY_A) {
			_moveDir.x = 0.0f;
		}
		else if (code == EventKeyboard::KeyCode::KEY_D) {
			_moveDir.x = 0.0f;
		}
		else if (code == EventKeyboard::KeyCode::KEY_W) {
			_moveDir.z = 0.0f;
		}
		else if (code == EventKeyboard::KeyCode::KEY_S) {
			_moveDir.z = 0.0f;
		}
	}

	//if (code == EventKeyboard::KeyCode::KEY_W
	//	|| code == EventKeyboard::KeyCode::KEY_S) {
	//	//_moveState = PlayerMoveState::STOP;
	//	TANK_MOVE_ON = false;
	//}

	CCLOG("onKeyReleased: %d", (int)code);
}

void PlayerController::update(float delta)
{
	if (_cannonState == CannonState::ROTATE_LEFT) {
		float angle = CONNON_ROTATE_TIME * CONNON_ROTATE_TIME;
		_player->rotateCannonStage(angle);
		_ovrRenderer->setOffsetRot(Quaternion(Vec3::UNIT_Y, CC_DEGREES_TO_RADIANS(angle)));
	}
	else if (_cannonState == CannonState::ROTATE_RIGHT) {
		float angle = CONNON_ROTATE_TIME * CONNON_ROTATE_TIME;
		_player->rotateCannonStage(-angle);
		_ovrRenderer->setOffsetRot(Quaternion(Vec3::UNIT_Y, CC_DEGREES_TO_RADIANS(-angle)));
	}


	if (_gunState == GunState::GUN_UP) {
		float angle = GUN_ROTATE_TIME * GUN_ROTATE_TIME;
		_player->rotateCannonGun(angle);
	}
	else if (_gunState == GunState::GUN_DOWN) {
		float angle = GUN_ROTATE_TIME * GUN_ROTATE_TIME;
		_player->rotateCannonGun(-angle);
	}

	if (_rotateState == PlayerRotateState::TURN) {
		//auto rot = Quaternion(Vec3::UNIT_Y, CC_DEGREES_TO_RADIANS(delta * TANK_TURN_SPEED));
		//_player->setRotationQuat(_player->getRotationQuat() * rot);
		if (0.0f < _moveDir.lengthSquared()) {
			auto rot = Quaternion(Vec3::UNIT_Y, CC_DEGREES_TO_RADIANS(_player->getTurnAngle()));
			Vec3 preMoveDir = rot * Vec3(0.0f, 0.0f, -1.0f);

			rot = Quaternion(Vec3::UNIT_Y, CC_DEGREES_TO_RADIANS(_player->getCannonStageAngle())) * rot;
			Vec3 movrDir = rot * _moveDir;
			movrDir.normalize();

			//Vec3 movrDir = _moveDir;

			float deltaAngle = CC_RADIANS_TO_DEGREES(acos(clampf(movrDir.dot(preMoveDir), 0.0f, 1.0f)));
			Vec3 rotDir;
			Vec3::cross(movrDir, preMoveDir, &rotDir);
			//CCLOG("angle: %f", deltaAngle);
			if (0.01f < abs(deltaAngle)) {
				float angle = deltaAngle * 0.02f;
				angle = deltaAngle < angle ? deltaAngle : angle;
				angle = 0.0f < rotDir.y ? -angle : angle;
				_player->turn(angle);
				_player->rotateCannonStage(-angle);
				_ovrRenderer->setOffsetRot(Quaternion(Vec3::UNIT_Y, CC_DEGREES_TO_RADIANS(-angle)));
				CCLOG("angle: %f", angle);
			}
		}
		//_ovrRenderer->setOffsetRot(rot);
	}
	//else if (_rotateState == PlayerRotateState::TURN_RIGHT) {
	//	//auto rot = Quaternion(Vec3::UNIT_Y, CC_DEGREES_TO_RADIANS(-delta * TANK_TURN_SPEED));
	//	//_player->setRotationQuat(_player->getRotationQuat() * rot);
	//	float angle = TANK_ROTATE_TIME * TANK_ROTATE_TIME;
	//	_player->turn(-angle);
	//	_player->rotateCannonStage(angle);
	//	_ovrRenderer->setOffsetRot(Quaternion(Vec3::UNIT_Y, CC_DEGREES_TO_RADIANS(angle)));
	//	//_ovrRenderer->setOffsetRot(rot);
	//}

	if (_moveState == PlayerMoveState::FRONT) {
		float moveSpeed = TANK_MOVE_TIME * TANK_MOVE_TIME;
		_player->move(moveSpeed);
	}
	//else if (_moveState == PlayerMoveState::BACK) {
	//	float moveSpeed = TANK_MOVE_TIME * TANK_MOVE_TIME;
	//	_player->move(-moveSpeed);
	//}

	if (CONNON_ROTATE_ON)
		CONNON_ROTATE_TIME += delta * CANNON_STAGE_ROTATE_SPEED;
	else
		CONNON_ROTATE_TIME -= delta * CANNON_STAGE_ROTATE_SPEED;

	if (CANNON_STAGE_MAX_ROTATE_SPEED < CONNON_ROTATE_TIME)
		CONNON_ROTATE_TIME = CANNON_STAGE_MAX_ROTATE_SPEED;

	if (CONNON_ROTATE_TIME < 0.0f) {
		CONNON_ROTATE_TIME = 0.0f;
		_cannonState = CannonState::STOP;
	}

	if (GUN_ROTATE_ON)
		GUN_ROTATE_TIME += delta * CANNON_GUN_ROTATE_SPEED;
	else
		GUN_ROTATE_TIME -= delta * CANNON_GUN_ROTATE_SPEED;

	if (CANNON_GUN_MAX_ROTATE_SPEED < GUN_ROTATE_TIME)
		GUN_ROTATE_TIME = CANNON_GUN_MAX_ROTATE_SPEED;

	if (GUN_ROTATE_TIME < 0.0f) {
		GUN_ROTATE_TIME = 0.0f;
		_gunState = GunState::STOP;
	}


	if (TANK_ROTATE_ON)
		TANK_ROTATE_TIME += delta * TANK_ROTATE_SPEED;
	else
		TANK_ROTATE_TIME -= delta * TANK_ROTATE_SPEED;

	if (TANK_MAX_ROTATE_SPEED < TANK_ROTATE_TIME)
		TANK_ROTATE_TIME = TANK_MAX_ROTATE_SPEED;

	if (TANK_ROTATE_TIME < 0.0f) {
		TANK_ROTATE_TIME = 0.0f;
		_rotateState = PlayerRotateState::STOP;
		TANK_MOVE_ON = false;
	}

	if (TANK_MOVE_ON)
		TANK_MOVE_TIME += delta * TANK_MOVE_SPEED;
	else
		TANK_MOVE_TIME -= delta * TANK_MOVE_SPEED;

	if (TANK_MAX_MOVE_SPEED < TANK_MOVE_TIME)
		TANK_MOVE_TIME = TANK_MAX_MOVE_SPEED;

	if (TANK_MOVE_TIME < 0.0f) {
		TANK_MOVE_TIME = 0.0f;
		_moveState = PlayerMoveState::STOP;
		SimpleAudioEngine::getInstance()->stopEffect(TANK_MOVE_SOUND);
	}
}

PlayerController* PlayerController::create()
{
	auto pc = new PlayerController();
	if (!pc->init()) {
		delete pc;
		return nullptr;
	}
	pc->autorelease();
	return pc;
}

bool PlayerController::init()
{
	auto keyboard = EventListenerKeyboard::create();
	keyboard->onKeyPressed = CC_CALLBACK_2(PlayerController::onKeyPressed, this);
	keyboard->onKeyReleased = CC_CALLBACK_2(PlayerController::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboard, this);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	auto controller = EventListenerController::create();
	controller->onConnected = CC_CALLBACK_2(PlayerController::onControllerConnected, this);
	controller->onDisconnected = CC_CALLBACK_2(PlayerController::onControllerDisconnected, this);
	controller->onKeyDown = CC_CALLBACK_3(PlayerController::onControllerKeyPressed, this);
	controller->onKeyUp = CC_CALLBACK_3(PlayerController::onControllerKeyReleased, this);
	controller->onAxisEvent = CC_CALLBACK_3(PlayerController::onControllerAxisEvent, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(controller, this); 
	Controller::startDiscoveryController();
#endif

	scheduleUpdate();

	return true;
}

void PlayerController::onControllerKeyPressed(Controller *controller, int key, Event *event)
{
	CCLOG("onControllerKeyPressed: %d", key);

	switch (key)
	{
	case Controller::Key::BUTTON_RIGHT_SHOULDER: {
		if (_player->shot(TANK_BULLET_SPEED)) {
			SimpleAudioEngine::getInstance()->playEffect(FileUtils::getInstance()->fullPathForFilename("sound/player_shot.mp3").c_str());
		}
	}
		break;
	case BUTTON_L2: {
		_moveState = PlayerMoveState::FRONT;
		TANK_MOVE_TIME = 0.0f;
		TANK_MOVE_ON = true;
	}
		break;
	case BUTTON_R2: {
		_moveState = PlayerMoveState::BACK;
		TANK_MOVE_TIME = 0.0f;
		TANK_MOVE_ON = true;
	}
		break;
	case Controller::Key::BUTTON_DPAD_LEFT:
	case Controller::Key::BUTTON_DPAD_RIGHT:
	case Controller::Key::BUTTON_DPAD_UP:
	case Controller::Key::BUTTON_DPAD_DOWN:
	{
		if (_rotateState != PlayerRotateState::TURN) {
			_rotateState = PlayerRotateState::TURN;
			TANK_ROTATE_TIME = 0.0f;
			TANK_ROTATE_ON = true;
		}

		if (_moveState != PlayerMoveState::FRONT) {
			_moveState = PlayerMoveState::FRONT;
			TANK_MOVE_TIME = 0.0f;
			TANK_MOVE_ON = true;
		}

		if (key == Controller::Key::BUTTON_DPAD_LEFT) {
			_moveDir.x = -1.0f;
		}
		else if (key == Controller::Key::BUTTON_DPAD_RIGHT) {
			_moveDir.x = 1.0f;
		}
		else if (key == Controller::Key::BUTTON_DPAD_UP) {
			_moveDir.z = -1.0f;
		}
		else if (key == Controller::Key::BUTTON_DPAD_DOWN) {
			_moveDir.z = 1.0f;
		}
	}
					break;
	default:
		break;
	}
}

void PlayerController::onControllerKeyReleased(Controller *controller, int key, Event *event)
{
	switch (key)
	{
	case BUTTON_L2: {
		TANK_MOVE_ON = false;
	}
		break;
	case BUTTON_R2: {
		TANK_MOVE_ON = false;
	}
		break;
	case Controller::Key::BUTTON_DPAD_LEFT:
	case Controller::Key::BUTTON_DPAD_RIGHT:
	case Controller::Key::BUTTON_DPAD_UP:
	case Controller::Key::BUTTON_DPAD_DOWN:
	{
		_moveDir = Vec3::ZERO;
		TANK_ROTATE_ON = false;
		TANK_MOVE_ON = false;

		if (key == Controller::Key::BUTTON_DPAD_LEFT) {
			_moveDir.x = 0.0f;
		}
		else if (key == Controller::Key::BUTTON_DPAD_RIGHT) {
			_moveDir.x = 0.0f;
		}
		else if (key == Controller::Key::BUTTON_DPAD_UP) {
			_moveDir.z = 0.0f;
		}
		else if (key == Controller::Key::BUTTON_DPAD_DOWN) {
			_moveDir.z = 0.0f;
		}
	}
	break;
	default:
		break;
	}

	CCLOG("onControllerKeyReleased: %d", key);
}

void PlayerController::onControllerAxisEvent(Controller *controller, int key, Event *event)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	const auto& ketStatus = controller->getKeyStatus(key);
	switch (key)
	{
	case Controller::Key::JOYSTICK_LEFT_X:{
		_moveDir.x = ketStatus.value;
		//if (ketStatus.value < 0) {
		//	if (_rotateState == PlayerRotateState::TURN)
		//		return;
		//	_rotateState = PlayerRotateState::TURN;
		//	TANK_ROTATE_TIME = 0.0f;
		//	TANK_ROTATE_ON = true;
		//}
		//else if (ketStatus.value > 0) {
		//	if (_rotateState == PlayerRotateState::TURN_RIGHT)
		//		return;
		//	_rotateState = PlayerRotateState::TURN_RIGHT;
		//	TANK_ROTATE_TIME = 0.0f;
		//	TANK_ROTATE_ON = true;
		//}
		//else {
		//	TANK_ROTATE_ON = false;
		//}
	}
		break;
	case Controller::Key::JOYSTICK_LEFT_Y: {
		_moveDir.z = ketStatus.value;
		//if (ketStatus.value < 0) {
		//	if (_moveState == PlayerMoveState::FRONT)
		//		return;
		//	_moveState = PlayerMoveState::FRONT;
		//	TANK_MOVE_TIME = 0.0f;
		//	TANK_MOVE_ON = true;
		//}
		//else if (ketStatus.value > 0) {
		//	if (_moveState == PlayerMoveState::BACK)
		//		return;
		//	_moveState = PlayerMoveState::BACK;
		//	TANK_MOVE_TIME = 0.0f;
		//	TANK_MOVE_ON = true;
		//}
		//else {
		//	TANK_MOVE_ON = false;
		//}
	}
	   break;
	case Controller::Key::JOYSTICK_RIGHT_X: {
		if (ketStatus.value < 0) {
			if (_cannonState == CannonState::ROTATE_LEFT)
				return;
			_cannonState = CannonState::ROTATE_LEFT;
			CONNON_ROTATE_TIME = 0.0f;
			CONNON_ROTATE_ON = true;
			CONNON_ROTATE_SOUND = SimpleAudioEngine::getInstance()->playEffect(FileUtils::getInstance()->fullPathForFilename("sound/cannon_rotate2.mp3").c_str(), true);
		}
		else if (ketStatus.value > 0) {
			if (_cannonState == CannonState::ROTATE_RIGHT)
				return;
			_cannonState = CannonState::ROTATE_RIGHT;
			CONNON_ROTATE_TIME = 0.0f;
			CONNON_ROTATE_ON = true;
			CONNON_ROTATE_SOUND = SimpleAudioEngine::getInstance()->playEffect(FileUtils::getInstance()->fullPathForFilename("sound/cannon_rotate2.mp3").c_str(), true);
		}else{
			CONNON_ROTATE_ON = false;
			SimpleAudioEngine::getInstance()->stopEffect(CONNON_ROTATE_SOUND);
		}
	}
		break;
	case Controller::Key::JOYSTICK_RIGHT_Y:{
		if (ketStatus.value < 0) {
			if (_gunState == GunState::GUN_UP)
				return;
			_gunState = GunState::GUN_UP;
			GUN_ROTATE_TIME = 0.0f;
			GUN_ROTATE_ON = true;
		}
		else if (ketStatus.value > 0) {
			if (_gunState == GunState::GUN_DOWN)
				return;
			_gunState = GunState::GUN_DOWN;
			GUN_ROTATE_TIME = 0.0f;
			GUN_ROTATE_ON = true;
		}
		else {
			GUN_ROTATE_ON = false;
		}
	}
		break;
	default:
		break;
	}

	if (0.0f < _moveDir.lengthSquared()) {
		if (_rotateState != PlayerRotateState::TURN) {
			_rotateState = PlayerRotateState::TURN;
			TANK_ROTATE_TIME = 0.0f;
			TANK_ROTATE_ON = true;
		}

		if (_moveState != PlayerMoveState::FRONT) {
			_moveState = PlayerMoveState::FRONT;
			TANK_MOVE_TIME = 0.0f;
			TANK_MOVE_ON = true;
		}
	}else {
		TANK_ROTATE_ON = false;
		TANK_MOVE_ON = false;
	}

	CCLOG("onControllerAxisEvent: %d", key);
#endif
}

void PlayerController::onControllerConnected(Controller *controller, Event *event)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	//receive back key
	controller->receiveExternalKeyEvent(4, true);
	//receive menu key
	controller->receiveExternalKeyEvent(82, true);
#endif
	CCLOG("onControllerConnected");
}

void PlayerController::onControllerDisconnected(Controller *controller, Event *event)
{
	CCLOG("onControllerDisconnected");
}
