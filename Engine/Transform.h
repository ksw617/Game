#pragma once
#include "Component.h"
					  
//��ȯ ����� �����ϴ� ����ü
struct TransformMatrix
{
	XMFLOAT4 offset; // ������ ���� �����ϴ� XMFLOAT4

};

class Transform	: public Component
{
public:
	//������ 
	//������Ʈ Ÿ���� TRANSFORM���� �����Ͽ� Component ������ ȣ��
	Transform() : Component(COMPONENT_TYPE::TRANSFORM) {}

	//�Ҹ���
	virtual ~Transform() {}
};

