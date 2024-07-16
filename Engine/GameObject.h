#pragma once
#include "Component.h"


class Transform;
class MeshFilter;
class Camera;

class MonoBehaviour;

//Object »ó¼Ó
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
public:

	void AddComponent(shared_ptr<Component> component);
};

