#include "pch.h"
#include "Component.h"
#include "GameObject.h"

//타입 초기화
Component::Component(COMPONENT_TYPE _type) : type(_type)
{
}

Component::~Component()
{
}

shared_ptr<GameObject> Component::GetGameObject()
{
    //weak_ptr 잠금하여 shared_ptr<GameObject>를 반환 
    return gameObject.lock();
}

shared_ptr<Transform> Component::GetTransform()
{
    //weak_ptr를 잠금하여 shared_ptr<GameObject>를 변환은 GetTransfrom함수를 이용해서
    //Transform을 반환
    return gameObject.lock()->GetTransform();
}
