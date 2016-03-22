#include "Tank.h"

USING_NS_CC;

#define BULLET_RELOAD_TIME 2.0
#define MOVE_AREA_RADIUS   150.0f

void recycleSetTexture(Node *node, Texture2D *texture)
{
	if (dynamic_cast<Sprite3D *>(node)) {
		dynamic_cast<Sprite3D *>(node)->setTexture(texture);
	}
	for (auto &child : node->getChildren()) {
		recycleSetTexture(child, texture);
	}
}

Tank::Tank()
	: _cannonStageAngle(180.0f)
	, _cannonGunAngle(0.0f)
	, _turnAngle(0.0f)
	, _hp(100.0f)
	, _latestShootTime(0.0f)
	, _needUpdateCannonStage(false)
	, _needUpdateGunAngle(false)
	, _state(0)
	, _shotDampingMotion(false)
	, _shotDampingTime(0.0f)
{

}

bool Tank::init()
{
	initWithFile(getModelFilePath());
	this->setRotation3D(Vec3(0.0f, 0.0f, 0.0f));
	this->setPosition3D(Vec3::ZERO);
	this->setScale(1.0f);

	auto aabb = this->getAABBRecursively();
	Vec3 size = Vec3(aabb._max - aabb._min);
	Physics3DRigidBodyDes rigidDes;
	rigidDes.mass = 0.0f;//ton
	rigidDes.originalTransform.setIdentity();
	rigidDes.shape = Physics3DShape::createBox(Vec3(size.x, size.y * 0.5f, size.z * 0.7f));
	auto rigid = Physics3DRigidBody::create(&rigidDes);
	rigid->setKinematic(true);
	rigid->setUserData(this);
	auto componet = Physics3DComponent::create(rigid, Vec3(0.0f, -size.y * 0.25f, size.z * 0.05f), Quaternion());
	this->addComponent(componet);
	componet->syncNodeToPhysics();
	//componet->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::NODE_TO_PHYSICS);

	_cannon = this->getChildByName("turret");
	_gunfire = PUParticleSystem3D::create("effects/Particle3D/scripts/shot.pu");
	_gunfire->setRotation3D(Vec3(90.0f, 0.0f, 0.0f));
	_gunfire->setScale(20.0f);
	_cannon->getChildByName("cannon")->getChildByName("Firepoint")->addChild(_gunfire);

	_pointLight = PointLight::create(Vec3::ZERO, Color3B(230, 136, 74), 10.0f);
	this->addChild(_pointLight);
	_pointLight->setLightFlag(LightFlag::LIGHT0);
	_pointLight->setIntensity(0.0f);


	NavMeshAgentParam param;
	param.radius = 3.0f;
	param.height = 4.0f;
	param.maxSpeed = 3.0f;
	_agent = NavMeshAgent::create(param);
	_agent->setOrientationRefAxes(-Vec3::UNIT_Z);
	this->addComponent(_agent);


	scheduleUpdate();
	return true;
}

bool Tank::initWithFile(const std::string& path)
{
	_aabbDirty = true;
	_meshes.clear();
	_meshVertexDatas.clear();
	CC_SAFE_RELEASE_NULL(_skeleton);
	removeAllAttachNode();

	if (loadFromCache(path))
		return true;

	MeshDatas* meshdatas = new (std::nothrow) MeshDatas();
	MaterialDatas* materialdatas = new (std::nothrow) MaterialDatas();
	NodeDatas* nodeDatas = new (std::nothrow) NodeDatas();
	if (loadFromFile(path, nodeDatas, meshdatas, materialdatas))
	{
		if (initFrom(*nodeDatas, *meshdatas, *materialdatas))
		{
			//add to cache
			auto data = new (std::nothrow) Sprite3DCache::Sprite3DData();
			data->materialdatas = materialdatas;
			data->nodedatas = nodeDatas;
			data->meshVertexDatas = _meshVertexDatas;
			for (const auto mesh : _meshes) {
				data->glProgramStates.pushBack(mesh->getGLProgramState());
			}

			Sprite3DCache::getInstance()->addSprite3DData(path, data);
			CC_SAFE_DELETE(meshdatas);
			_contentSize = getBoundingBox().size;
			return true;
		}
	}
	CC_SAFE_DELETE(meshdatas);
	CC_SAFE_DELETE(materialdatas);
	CC_SAFE_DELETE(nodeDatas);

	return false;
}

bool Tank::initFrom(const NodeDatas& nodeDatas, const MeshDatas& meshdatas, const MaterialDatas& materialdatas)
{
	for (const auto& it : meshdatas.meshDatas)
	{
		if (it)
		{
			//            Mesh* mesh = Mesh::create(*it);
			//            _meshes.pushBack(mesh);
			auto meshvertex = MeshVertexData::create(*it);
			_meshVertexDatas.pushBack(meshvertex);
		}
	}
	_skeleton = Skeleton3D::create(nodeDatas.skeleton);
	CC_SAFE_RETAIN(_skeleton);

	for (const auto& it : nodeDatas.nodes)
	{
		if (it)
		{
			createNode(it, this, materialdatas, nodeDatas.nodes.size() == 1);
		}
	}
	for (const auto& it : nodeDatas.skeleton)
	{
		if (it)
		{
			createAttachSprite3DNode(it, materialdatas);
		}
	}
	genMaterial();

	return true;
}

void Tank::createNode(NodeData* nodedata, Node* root, const MaterialDatas& materialdatas, bool singleSprite)
{
	Node* node = nullptr;
	for (const auto& it : nodedata->modelNodeDatas)
	{
		if (it)
		{
			//if(it->bones.size() > 0 || singleSprite)
			//{
			//    if(singleSprite && root!=nullptr)
			//        root->setName(nodedata->id);
			//    auto mesh = Mesh::create(nodedata->id, getMeshIndexData(it->subMeshId));
			//    if(mesh)
			//    {
			//        _meshes.pushBack(mesh);
			//        if (_skeleton && it->bones.size())
			//        {
			//            auto skin = MeshSkin::create(_skeleton, it->bones, it->invBindPose);
			//            mesh->setSkin(skin);
			//        }
			//        mesh->_visibleChanged = std::bind(&Sprite3D::onAABBDirty, this);
			//        if (it->matrialId == "" && materialdatas.materials.size())
			//        {
			//            const NTextureData* textureData = materialdatas.materials[0].getTextureData(NTextureData::Usage::Diffuse);
			//            mesh->setTexture(textureData->filename);
			//        }
			//        else
			//        {
			//            const NMaterialData*  materialData=materialdatas.getMaterialData(it->matrialId);
			//            if(materialData)
			//            {
			//                const NTextureData* textureData = materialData->getTextureData(NTextureData::Usage::Diffuse);
			//                if(textureData)
			//                {
			//                    mesh->setTexture(textureData->filename);
			//                    auto tex = mesh->getTexture();
			//                    if(tex)
			//                    {
			//                        Texture2D::TexParams texParams;
			//                        texParams.minFilter = GL_LINEAR;
			//                        texParams.magFilter = GL_LINEAR;
			//                        texParams.wrapS = textureData->wrapS;
			//                        texParams.wrapT = textureData->wrapT;
			//                        tex->setTexParameters(texParams);
			//                        mesh->_isTransparent = (materialData->getTextureData(NTextureData::Usage::Transparency) != nullptr);
			//                    }
			//                }
			//            }
			//        }
			//        
			//        Vec3 pos;
			//        Quaternion qua;
			//        Vec3 scale;
			//        nodedata->transform.decompose(&scale, &qua, &pos);
			//        setPosition3D(pos);
			//        setRotationQuat(qua);
			//        setScaleX(scale.x);
			//        setScaleY(scale.y);
			//        setScaleZ(scale.z);
			//        
			//    }
			//}
			//else
			{
				auto sprite = createSprite3DNode(nodedata, it, materialdatas);
				if (sprite)
				{
					if (root)
					{
						root->addChild(sprite);
					}
				}
				node = sprite;
			}
		}
	}
	if (nodedata->modelNodeDatas.size() == 0)
	{
		node = Node::create();
		if (node)
		{
			node->setName(nodedata->id);

			// set locale transform
			Vec3 pos;
			Quaternion qua;
			Vec3 scale;
			nodedata->transform.decompose(&scale, &qua, &pos);
			node->setPosition3D(pos);
			node->setRotationQuat(qua);
			node->setScaleX(scale.x);
			node->setScaleY(scale.y);
			node->setScaleZ(scale.z);

			if (root)
			{
				root->addChild(node);
			}
		}
	}
	for (const auto& it : nodedata->children)
	{
		createNode(it, node, materialdatas, nodedata->children.size() == 1);
	}
}

bool Tank::shot(float speed)
{
	if (BULLET_RELOAD_TIME < _latestShootTime) {
		shotBullet(speed);
		_gunfire->startParticleSystem();
		_latestShootTime = 0.0f;

		auto mat = this->getWorldToNodeTransform() * _cannon->getChildByName("cannon")->getChildByName("Firepoint")->getNodeToWorldTransform();
		_pointLight->setPosition3D(Vec3(mat.m[12], mat.m[13], mat.m[14]));
		_pointLight->setIntensity(5.0f);

		_shotDampingMotion = true;
		_shotDampingTime = 0.0f;
		return true;
	}
	return false;
}

bool Tank::shot(const Vec3 &target, float speed)
{
	float dis = (target - this->getPosition3D()).length();
	float gunAngle = dis / 15.0f - 6.0f;
	rotateCannonGun(gunAngle - _cannonGunAngle);

	Vec3 dir = target - this->getPosition3D();
	dir.normalize();
	Quaternion rot = Quaternion(Vec3::UNIT_Y, CC_DEGREES_TO_RADIANS(_cannonStageAngle - 180.0f)) * this->getRotationQuat();
	Vec3 gunDir = rot * -Vec3::UNIT_Z;
	gunDir.normalize();
	float theta = acos(clampf(gunDir.dot(dir), 0.0f, 1.0f));
	Vec3 up;
	Vec3::cross(gunDir, dir, &up);
	if (up.y < 0.0)
		theta = -theta;
	rotateCannonStage(CC_RADIANS_TO_DEGREES(theta));
	return shot(speed);
}

void Tank::rotateCannonStage(float angle)
{
	_cannonStageAngle += angle;
	_needUpdateCannonStage = true;
}

void Tank::rotateCannonGun(float angle)
{
	_cannonGunAngle += angle;
	if (10.0f < _cannonGunAngle) _cannonGunAngle = 10.0f;
	if (_cannonGunAngle < -5.0f) _cannonGunAngle = -5.0f;
	_needUpdateGunAngle = true;
}

bool Tank::move(float force)
{
	Vec3 requestPos = Vec3(this->getPosition3D() + this->getRotationQuat() * Vec3(0.0f, 0.0f, -force));
	if (true/*requestPos.length() < MOVE_AREA_RADIUS*/) {
		this->setPosition3D(requestPos);
		changeLeftTrackTexture(force * 7.0f);
		changeRightTrackTexture(force * 7.0f);
		return true;
	}
	return false;
}

bool Tank::move(const Vec3 &target)
{
	_agent->move(target, [this](NavMeshAgent *agent, float totalTimeAfterMove) {
		static float preTime = 0.0f;
		changeLeftTrackTexture((totalTimeAfterMove - preTime) * 7.0f);
		changeRightTrackTexture((totalTimeAfterMove - preTime) * 7.0f);
		preTime = totalTimeAfterMove;
	});

	return true;
}

void Tank::turn(float torque)
{
	_turnAngle += torque;
	auto rot = Quaternion(Vec3::UNIT_Y, CC_DEGREES_TO_RADIANS(torque));
	this->setRotationQuat(this->getRotationQuat() * rot);
	changeLeftTrackTexture(-torque);
	changeRightTrackTexture(torque);
}

void Tank::shotBullet(float speed)
{
	Mat4 bulletWorldMat = _cannon->getChildByName("cannon")->getChildByName("Firepoint")->getNodeToWorldTransform();
	Vec3 bulletPos;
	Quaternion bulletRot;
	bulletWorldMat.decompose(nullptr, &bulletRot, &bulletPos);

	Physics3DRigidBodyDes rbDes;
	rbDes.mass = 10.f;
	rbDes.shape = Physics3DShape::createCapsule(0.1f, 0.2f);
	auto bullet = PhysicsSprite3D::create("models/bullet.c3b", &rbDes);
	bullet->setTexture("models/orange_edit.png");
	Director::getInstance()->getRunningScene()->addChild(bullet);

	Vec3 rotate = this->getRotation3D();

	Vec3 linearVel = Vec3(cos(CC_DEGREES_TO_RADIANS(_cannonGunAngle)) * sin(CC_DEGREES_TO_RADIANS(_cannonStageAngle))
		, sin(CC_DEGREES_TO_RADIANS(_cannonGunAngle))
		, cos(CC_DEGREES_TO_RADIANS(_cannonGunAngle)) * cos(CC_DEGREES_TO_RADIANS(_cannonStageAngle)));

	linearVel = this->getRotationQuat() * linearVel;

	auto rigidBody = static_cast<Physics3DRigidBody*>(bullet->getPhysicsObj());
	rigidBody->setLinearFactor(Vec3::ONE);
	rigidBody->setLinearVelocity(linearVel * speed);
	rigidBody->setAngularVelocity(Vec3::ZERO);
	rigidBody->setCcdMotionThreshold(0.5f);
	rigidBody->setCcdSweptSphereRadius(0.4f);
	//rigidBody->setMask(0xffffffff);
	rigidBody->setUserData(bullet);

	bullet->setCameraMask(this->getCameraMask());
	bullet->setPosition3D(bulletPos);
	bullet->setRotationQuat(Quaternion(Vec3::UNIT_Y, CC_DEGREES_TO_RADIANS(_cannonStageAngle)) * this->getRotationQuat() * Quaternion(Vec3::UNIT_X, CC_DEGREES_TO_RADIANS(-_cannonGunAngle + 85.0f)));
	//bullet->setRotation3D(this->getRotationQuat() * Vec3(0.0f, _cannonStageAngle, 0.0f) + Vec3(-_cannonGunAngle + 85.0f, 0.0f, 0.0f));
	bullet->syncNodeToPhysics();
	bullet->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::PHYSICS_TO_NODE);


	rigidBody->setCollisionCallback([bullet, this](const Physics3DCollisionInfo &ci) {
		if (!ci.collisionPointList.empty()) {
			bullet->runAction(CallFunc::create([bullet]() {
				bullet->removeFromParent();
			}));
			if (this->_callBack != nullptr) {
				this->_callBack(ci);
			}
		}
	});
}

void Tank::setTexture(cocos2d::Texture2D *texture)
{
	recycleSetTexture(_cannon, texture);
	recycleSetTexture(this->getChildByName("body"), texture);
}

void Tank::setHP(float hp)
{
	if (hp < 0.0)
		_hp = 0.0;
	else
		_hp = hp;
}

void Tank::update(float delta)
{
	_latestShootTime += delta;
	if (_needUpdateCannonStage) {
		_cannon->setRotation3D(Vec3(-90.0f, _cannonStageAngle, 0.0f));
		_needUpdateCannonStage = false;
	}

	if (_needUpdateGunAngle) {
		auto gun = _cannon->getChildByName("cannon");
		gun->setRotation3D(Vec3(-_cannonGunAngle, 0.0f, 0.0f));
		_needUpdateGunAngle = false;
	}

	if (0.0f < _pointLight->getIntensity()) {
		float intensity = _pointLight->getIntensity() - 2.5f * delta;
		_pointLight->setIntensity(0.0f <= intensity? intensity: 0.0f);
	}

	if (_shotDampingMotion) {
		static const float PI = 3.1415926f;
		static float A = 0.05f;
		static float beta = 0.6f;
		static float w0 = PI / 4.0f;
		static float w = sqrt(w0 * w0 - beta * beta);
		static float alpha = 0.0f;
		float x = A * exp(-beta * _shotDampingTime) * cos(w * _shotDampingTime + alpha);

		auto rot = this->getRotationQuat() * Quaternion(Vec3::UNIT_Y, CC_DEGREES_TO_RADIANS(_cannonStageAngle - 180.0f));

		Vec3 requestPos = this->getPosition3D() + rot * Vec3(0.0f, 0.0f, x);
		if (requestPos.length() < MOVE_AREA_RADIUS) {
			this->setPosition3D(requestPos);
		}
		_shotDampingTime += delta * 5.0f;
		if (abs(x) < 0.01f) {
			_shotDampingMotion = false;
		}
	}
}

void Tank::changeLeftTrackTexture(float step)
{
	static float currentIdx = 0.0f;
	currentIdx += step;
	currentIdx = fmod(currentIdx, 4.0f);
	currentIdx = currentIdx < 0.0f ? 4.0f + currentIdx : currentIdx;
	static_cast<Sprite3D *>(this->getChildByName("Lefttrack"))->setTexture(_trackTextures[(int)currentIdx]);
}

void Tank::changeRightTrackTexture(float step)
{
	static float currentIdx = 0.0f;
	currentIdx += step;
	currentIdx = fmod(currentIdx, 4.0f);
	currentIdx = currentIdx < 0.0f ? 4.0f + currentIdx : currentIdx;
	static_cast<Sprite3D *>(this->getChildByName("Righttrack"))->setTexture(_trackTextures[(int)currentIdx]);
}

cocos2d::Node * Tank::getCannonStageNode() const
{
	return _cannon;
}
