#pragma once
#include "MonoBehaviour.h"
class CameraMoveTest : public MonoBehaviour
{
private:
	float speed = 100.f;
public:
	CameraMoveTest() {}
	virtual ~CameraMoveTest() {}
public:
	//LateUpdate 함수 호출
	virtual void LateUpdate() override;

};

