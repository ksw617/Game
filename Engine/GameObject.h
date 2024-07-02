#pragma once
#include "Component.h"

class Transform;
class MonoBehaviour;

class GameObject : public enable_shared_from_this<GameObject>
{
private:
	array<shared_ptr<Component>, FIXED_COMPONENT_COUNT> components;
	vector<shared_ptr<MonoBehaviour>> scripts;	//MonoBehaviour를 상속 받는 스크립트들을 담을 vector
public:
	GameObject();
	virtual ~GameObject();
public:
	void Init();
public:	
	void Awake();
	void Start();
	void Update();
	void LateUpdate();
public:
	shared_ptr<Transform> GetTransform();
	void AddComponent(shared_ptr<Component> component);
};

