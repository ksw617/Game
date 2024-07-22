#include "pch.h"
#include "Light.h"
#include "Transform.h" // Transform 포함

void Light::FinalUpdate()
{
	//Transform 컴포넌트에서 월드 좌표를 가져와 조명 정보의 위치에 설정
	lightInfo.position = Vec3ToVec4(GetTransform()->GetWorldPosition());
}
