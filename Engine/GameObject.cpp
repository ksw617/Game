#include "pch.h"
#include "GameObject.h"
#include "Transform.h"
#include "MeshFilter.h"
#include "Camera.h"
#include "MonoBehaviour.h"

GameObject::GameObject()
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
	//Transform 타입의 컴포넌트를 가져옴
	shared_ptr<Component> component = GetFixedComponent(COMPONENT_TYPE::TRANSFORM);

	//Component 포인터를 transform 포인터로 캐스팅하여 반환
	return static_pointer_cast<Transform>(component);
}

shared_ptr<MeshFilter> GameObject::GetMeshFilter()
{
	//MeshFilter 타입의 컴포넌트를 가져옴
	shared_ptr<Component> component = GetFixedComponent(COMPONENT_TYPE::MESH_FILTER);

	//Component 포인터를 MeshFilter 포인터로 캐스팅하여 반환
	return static_pointer_cast<MeshFilter>(component);
}

shared_ptr<Camera> GameObject::GetCamera()
{
	//Camera 타입의 컴포넌트를 가져옴
	shared_ptr<Component> component = GetFixedComponent(COMPONENT_TYPE::CAMERA);

	//Component 포인터를 Camera 포인터로 캐스팅하여 반환
	return static_pointer_cast<Camera>(component);
}


shared_ptr<Component> GameObject::GetFixedComponent(COMPONENT_TYPE type)
{
	//컴포넌트 타입을 정수 인덱스로 변환
	UINT8 index = static_cast<UINT8>(type);

	//인덱스가 정해진 컴포넌트의 갯수를 초과하지 않는지 확인
	assert(index < FIXED_COMPONENT_COUNT);

	//지정된 인덱스의 컴포넌트를 반환
	return components[index];
}