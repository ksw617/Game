#include "pch.h"
#include "GameObject.h"
#include "Transform.h"
#include "MeshFilter.h"
#include "Camera.h"
#include "Light.h" // Light 포함
#include "MonoBehaviour.h"


GameObject::GameObject() : Object(OBJECT_TYPE::GAMEOBJECT)
{
}

GameObject::~GameObject()
{
}

void GameObject::Init()
{
	AddComponent(make_shared<Transform>());
}

void GameObject::AddComponent(shared_ptr<Component> component)
{
	component->SetGameObject(shared_from_this());

	UINT8 index = static_cast<UINT8>(component->GetType());
	if (index < FIXED_COMPONENT_COUNT)
	{
		components[index] = component;

	}
	else
	{
		scripts.push_back(dynamic_pointer_cast<MonoBehaviour>(component));
	}
}


void GameObject::Awake()
{													  
	for (shared_ptr<Component>& component : components)
	{
		if (component)
			component->Awake();
	}

	for (shared_ptr<MonoBehaviour>& script : scripts)
	{
		script->Awake();
	}
}

void GameObject::Start()
{
	for (shared_ptr<Component>& component : components)
	{
		if (component)
			component->Start();
	}

	for (shared_ptr<MonoBehaviour>& script : scripts)
	{
		script->Start();
	}
}

void GameObject::Update()
{
	for (shared_ptr<Component>& component : components)
	{
		if (component)
			component->Update();
	}

	for (shared_ptr<MonoBehaviour>& script : scripts)
	{
		script->Update();
	}
}

void GameObject::LateUpdate()
{
	for (shared_ptr<Component>& component : components)
	{
		if (component)
			component->LateUpdate();
	}

	for (shared_ptr<MonoBehaviour>& script : scripts)
	{
		script->LateUpdate();
	}
}

void GameObject::FinalUpdate()
{
	for (shared_ptr<Component>& component : components)
	{
		if (component)
			component->FinalUpdate();
	}
}

shared_ptr<Transform> GameObject::GetTransform()
{
	shared_ptr<Component> component = GetFixedComponent(COMPONENT_TYPE::TRANSFORM);
	return static_pointer_cast<Transform>(component);
}

shared_ptr<MeshFilter> GameObject::GetMeshFilter()
{
	shared_ptr<Component> component = GetFixedComponent(COMPONENT_TYPE::MESH_FILTER);
	return static_pointer_cast<MeshFilter>(component);
}

shared_ptr<Camera> GameObject::GetCamera()
{
	shared_ptr<Component> component = GetFixedComponent(COMPONENT_TYPE::CAMERA);
	return static_pointer_cast<Camera>(component);
}


//Light 반환
shared_ptr<Light> GameObject::GetLight()
{

	shared_ptr<Component> component = GetFixedComponent(COMPONENT_TYPE::LIGHT);
	return static_pointer_cast<Light>(component);
}


shared_ptr<Component> GameObject::GetFixedComponent(COMPONENT_TYPE type)
{
	UINT8 index = static_cast<UINT8>(type);
	assert(index < FIXED_COMPONENT_COUNT);
	return components[index];
}
