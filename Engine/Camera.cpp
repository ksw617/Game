#include "pch.h"
#include "Camera.h"
#include "Engine.h"
#include "Transform.h"

//정적 변수 초기화
Matrix Camera::StaticMatrixView;
Matrix Camera::StaticMatrixProjection;

//카메라 컴포넌트 생성자 : 컴포넌트 타입을 CAMERA로 설정
Camera::Camera() : Component(COMPONENT_TYPE::CAMERA)
{
}

Camera::~Camera()
{
}

void Camera::FinalUpdate()
{
	//뷰 행렬 계산: 변환 컴포넌트의 로컬 월드 변환 행렬을 가져와서 역행렬을 구함
	matrixView = GetTransform()->GetLocalToWorldMatrix().Invert();

	//화면의 너비와 높이를 float으로 가져옴
	float width = static_cast<float>(Engine::Get().GetWidth());
	float height = static_cast<float>(Engine::Get().GetHeight());

	//카메라 타입에 따라 투영 행렬 계산
	if (type == PROJECTION_TYPE::PERSPECTIVE)
		//원근 투영 행렬을 계산
		matrixProjection = XMMatrixPerspectiveFovLH(fov, width / height, nearView, farView);
	else
		//직교 투영 행렬을 계산
		matrixProjection = XMMatrixOrthographicLH(width * size, height * size, nearView, farView);

	//정적 변수에 뷰 행렬과 투영 행렬을 저장
	StaticMatrixView = matrixView;
	StaticMatrixProjection = matrixProjection;
}