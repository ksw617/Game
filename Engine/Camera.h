#pragma once
#include "Component.h"

enum class PROJECTION_TYPE : UINT8
{	
	PERSPECTIVE,	// 원근 투영
	ORTHOGRAPHIC,	// 직교 투영

};

class Camera : public Component
{
private:
	PROJECTION_TYPE type = PROJECTION_TYPE::PERSPECTIVE; // 카메라 타입(기본값은 원근 투영)

	float nearView = 0.3f;	//근경 평면 거리
	float farView = 1000.f;	//원경 평면 거리
	float fov = XM_PI / 4.f;//시야각 (FOV)
	float size = 1.f;		//크기값 (직교 투영에 사용)

	Matrix matrixView = {};			//뷰 행렬
	Matrix matrixProjection = {};	//투영 행렬
public:
	//임시로 사용되는 정적 변수들-> MainCamera
	static Matrix StaticMatrixView;			//정적 뷰 행렬
	static Matrix StaticMatrixProjection;	//정적 투영 행렬
public:
	//생성자
	Camera();
	//소멸자
	virtual ~Camera();

public:
	// 최종 업데이트 함수(매 프레임 호출됨)
	virtual void FinalUpdate() override;
};

