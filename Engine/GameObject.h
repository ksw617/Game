#pragma once
#include "Component.h"


class Transform;
class MeshFilter;
class Camera;
class Light; //Light 클라스 전방선언

class MonoBehaviour;

class GameObject : public enable_shared_from_this<GameObject>, public Object
{
private:
	array<shared_ptr<Component>, FIXED_COMPONENT_COUNT> components;
	vector<shared_ptr<MonoBehaviour>> scripts;
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
	void FinalUpdate();
public:
	shared_ptr<Transform> GetTransform();
	shared_ptr<MeshFilter> GetMeshFilter();
	shared_ptr<Camera> GetCamera();
	shared_ptr<Component> GetFixedComponent(COMPONENT_TYPE type);
	shared_ptr<Light> GetLight(); // Light Get 함수
public:

	void AddComponent(shared_ptr<Component> component);
};

