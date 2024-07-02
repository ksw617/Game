#include "pch.h"
#include "GameObject.h"
#include "Transform.h"
#include "MeshFilter.h"
#include "MonoBehaviour.h" // 추가

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
	else //FIXED_COMPONENT_COUNT를 벗어 나면 Monobehaviour를 상속 받은 애들이니까 커스텀 스크립트에 추가
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

	//모든 컴퍼넌트를 순회하며 Awake 함수 호출
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

	//모든 컴퍼넌트를 순회하며 Start 함수 호출
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

	//모든 컴퍼넌트를 순회하며 Update 함수 호출
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

	//모든 컴퍼넌트를 순회하며 LateUpdate 함수 호출
	for (shared_ptr<MonoBehaviour>& script : scripts)
	{
		script->LateUpdate();
	}
}

shared_ptr<Transform> GameObject::GetTransform()
{
	UINT8 index = static_cast<UINT8>(COMPONENT_TYPE::TRANSFORM);
	return static_pointer_cast<Transform>(components[index]);
}
