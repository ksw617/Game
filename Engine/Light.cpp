#include "pch.h"
#include "Light.h"
#include "Transform.h" // Transform ����

void Light::FinalUpdate()
{
	//Transform ������Ʈ���� ���� ��ǥ�� ������ ���� ������ ��ġ�� ����
	lightInfo.position = Vec3ToVec4(GetTransform()->GetWorldPosition());
}
