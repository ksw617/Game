#include "pch.h"
#include "GameObject.h"
#include "Transform.h"
#include "MeshFilter.h"
#include "MonoBehaviour.h" // �߰�

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
	else //FIXED_COMPONENT_COUNT�� ���� ���� Monobehaviour�� ��� ���� �ֵ��̴ϱ� Ŀ���� ��ũ��Ʈ�� �߰�
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

	//��� ���۳�Ʈ�� ��ȸ�ϸ� Awake �Լ� ȣ��
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

	//��� ���۳�Ʈ�� ��ȸ�ϸ� Start �Լ� ȣ��
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

	//��� ���۳�Ʈ�� ��ȸ�ϸ� Update �Լ� ȣ��
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

	//��� ���۳�Ʈ�� ��ȸ�ϸ� LateUpdate �Լ� ȣ��
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
