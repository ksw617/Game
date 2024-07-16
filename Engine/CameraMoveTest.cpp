#include "pch.h"
#include "CameraMoveTest.h"
#include "Transform.h"
#include "Engine.h"

void CameraMoveTest::LateUpdate()
{
	//현재 위치를 가져옴
	Vector3 pos = GetTransform()->GetLocalPosition();

	//W키를 눌렀을 때 카메라를 앞으로 이동
	if (Engine::Get().GetInput()->GetButton(KEY_TYPE::W))
		pos += GetTransform()->GetLook() * speed * Engine::Get().GetTimer()->GetDeltaTime();

	//S키를 눌렀을 때 카메라를 뒤로 이동
	if (Engine::Get().GetInput()->GetButton(KEY_TYPE::S))
		pos -= GetTransform()->GetLook() * speed * Engine::Get().GetTimer()->GetDeltaTime();

	//A키를 눌렀을 때 카메라를 왼쪽 으로 이동
	if (Engine::Get().GetInput()->GetButton(KEY_TYPE::A))
		pos -= GetTransform()->GetRight() * speed * Engine::Get().GetTimer()->GetDeltaTime();

	//D키를 눌렀을 때 카메라를 오른쪽 으로 이동
	if (Engine::Get().GetInput()->GetButton(KEY_TYPE::D))
		pos += GetTransform()->GetRight() * speed * Engine::Get().GetTimer()->GetDeltaTime();


	//Q키를 눌렀을 때 카메라를 위로 회전
	if (Engine::Get().GetInput()->GetButton(KEY_TYPE::Q))
	{
		//현재 회전 값을 가져와서 X축 회전을 증가시킴
		Vector3 rotation = GetTransform()->GetLocalRotation();
		rotation.x += Engine::Get().GetTimer()->GetDeltaTime() * 0.5f;
		GetTransform()->SetLocalRotation(rotation);
		
	}

	//키를 눌렀을 때 카메라를 아래로 회전
	if (Engine::Get().GetInput()->GetButton(KEY_TYPE::E))
	{
		//현재 회전 값을 가져와서 X축 회전을 감소시킴
		Vector3 rotation = GetTransform()->GetLocalRotation();
		rotation.x -= Engine::Get().GetTimer()->GetDeltaTime() * 0.5f;
		GetTransform()->SetLocalRotation(rotation);

	}
		
	//변경된 위치로 설정
	GetTransform()->SetLocalPosition(pos);
		
}
