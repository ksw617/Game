#include "pch.h"
#include "Component.h"
#include "GameObject.h"

//Ÿ�� �ʱ�ȭ
Component::Component(COMPONENT_TYPE _type) : type(_type)
{
}

Component::~Component()
{
}

shared_ptr<GameObject> Component::GetGameObject()
{
    //weak_ptr ����Ͽ� shared_ptr<GameObject>�� ��ȯ 
    return gameObject.lock();
}

shared_ptr<Transform> Component::GetTransform()
{
    //weak_ptr�� ����Ͽ� shared_ptr<GameObject>�� ��ȯ�� GetTransfrom�Լ��� �̿��ؼ�
    //Transform�� ��ȯ
    return gameObject.lock()->GetTransform();
}
