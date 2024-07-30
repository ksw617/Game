#pragma once
#include "Component.h"


class Transform;
class MeshFilter;
class Camera;
class Light; 

class MonoBehaviour;

class GameObject : public enable_shared_from_this<GameObject>, public Object
{
private:
	//���̾� index �߰� defualt ���� 0����
	UINT8 layerIndex = 0;
	bool isCullingMask = true;
	array<shared_ptr<Component>, FIXED_COMPONENT_COUNT> components;
	vector<shared_ptr<MonoBehaviour>> scripts;
public:
	GameObject();
	virtual ~GameObject();
public:
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
	shared_ptr<Light> GetLight();

	void SetCullingMask(bool enable) { isCullingMask = enable; }
	bool GetCullingMask() const { return isCullingMask; }

	 //layerIndex Get & Set �Լ�
	void SetLayerIndex(UINT8 layer) { layerIndex = layer; }
	UINT8 GetLayerIndex() const { return layerIndex; }
public:					  

	void AddComponent(shared_ptr<Component> component);
};

