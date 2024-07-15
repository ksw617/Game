#pragma once
#include "Component.h"


class Transform;
//전방선언
class MeshFilter;
class Camera;

class MonoBehaviour;

class GameObject : public enable_shared_from_this<GameObject>
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
	//MeshFilter Get 함수
	shared_ptr<MeshFilter> GetMeshFilter();
	//Camera Get 함수
	shared_ptr<Camera> GetCamera();

	//필요한 컴포넌트들만 반환
	shared_ptr<Component> GetFixedComponent(COMPONENT_TYPE type);

	void AddComponent(shared_ptr<Component> component);
};

