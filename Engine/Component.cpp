#include "pch.h"
#include "Component.h"
#include "GameObject.h"

//타입 초기화
Component::Component(COMPONENT_TYPE _type) : componentType(_type), Object(OBJECT_TYPE::COMPONENT)
{
}

Component::~Component()
{
}

shared_ptr<GameObject> Component::GetGameObject()
{
    return gameObject.lock();
}

shared_ptr<Transform> Component::GetTransform()
{
    return gameObject.lock()->GetTransform();
}
