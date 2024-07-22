#pragma once
#include "Object.h"

enum class COMPONENT_TYPE : UINT8
{
	TRANSFORM,		
	MESH_FILTER,	
	CAMERA, 
	LIGHT, // LIGHT Ãß°¡

	MONO_BEHAVIOUR, 

	END,			

};

enum
{
	FIXED_COMPONENT_COUNT = static_cast<UINT8>(COMPONENT_TYPE::END) - 1 
};

class GameObject;
class Transform;

class Component	: public Object
{
public:
	Component(COMPONENT_TYPE _type);
	virtual ~Component();
protected:
	COMPONENT_TYPE type;
	weak_ptr<GameObject> gameObject;
private:
	friend class GameObject;	
	void SetGameObject(shared_ptr<GameObject> _gameObject) { gameObject = _gameObject; }
public:
	COMPONENT_TYPE GetType() const { return type; }
	bool IsValid() { return gameObject.expired() == false; }
public:
	virtual void Awake() {}
	virtual void Start() {}
	virtual void Update() {}
	virtual void LateUpdate() {}
	virtual void FinalUpdate() {}

public:
	shared_ptr<GameObject> GetGameObject();
	shared_ptr<Transform> GetTransform();
};

