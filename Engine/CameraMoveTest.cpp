#include "pch.h"
#include "CameraMoveTest.h"
#include "Transform.h"
#include "Engine.h"

void CameraMoveTest::LateUpdate()
{
	//���� ��ġ�� ������
	Vector3 pos = GetTransform()->GetLocalPosition();

	//WŰ�� ������ �� ī�޶� ������ �̵�
	if (Engine::Get().GetInput()->GetButton(KEY_TYPE::W))
		pos += GetTransform()->GetLook() * speed * Engine::Get().GetTimer()->GetDeltaTime();

	//SŰ�� ������ �� ī�޶� �ڷ� �̵�
	if (Engine::Get().GetInput()->GetButton(KEY_TYPE::S))
		pos -= GetTransform()->GetLook() * speed * Engine::Get().GetTimer()->GetDeltaTime();

	//AŰ�� ������ �� ī�޶� ���� ���� �̵�
	if (Engine::Get().GetInput()->GetButton(KEY_TYPE::A))
		pos -= GetTransform()->GetRight() * speed * Engine::Get().GetTimer()->GetDeltaTime();

	//DŰ�� ������ �� ī�޶� ������ ���� �̵�
	if (Engine::Get().GetInput()->GetButton(KEY_TYPE::D))
		pos += GetTransform()->GetRight() * speed * Engine::Get().GetTimer()->GetDeltaTime();


	//QŰ�� ������ �� ī�޶� ���� ȸ��
	if (Engine::Get().GetInput()->GetButton(KEY_TYPE::Q))
	{
		//���� ȸ�� ���� �����ͼ� X�� ȸ���� ������Ŵ
		Vector3 rotation = GetTransform()->GetLocalRotation();
		rotation.x += Engine::Get().GetTimer()->GetDeltaTime() * 0.5f;
		GetTransform()->SetLocalRotation(rotation);
		
	}

	//Ű�� ������ �� ī�޶� �Ʒ��� ȸ��
	if (Engine::Get().GetInput()->GetButton(KEY_TYPE::E))
	{
		//���� ȸ�� ���� �����ͼ� X�� ȸ���� ���ҽ�Ŵ
		Vector3 rotation = GetTransform()->GetLocalRotation();
		rotation.x -= Engine::Get().GetTimer()->GetDeltaTime() * 0.5f;
		GetTransform()->SetLocalRotation(rotation);

	}
		
	//����� ��ġ�� ����
	GetTransform()->SetLocalPosition(pos);
		
}
