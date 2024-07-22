#include "pch.h"
#include "Transform.h"
#include "Camera.h"
#include "Engine.h"

void Transform::FinalUpdate()
{
	Matrix matrixScale = Matrix::CreateScale(localScale);

	Matrix matrixRotation = Matrix::CreateRotationX(localRotation.x);
	matrixRotation *= Matrix::CreateRotationY(localRotation.y);
	matrixRotation *= Matrix::CreateRotationZ(localRotation.z);

	Matrix matrixTranslation = Matrix::CreateTranslation(localPosition);

	matrixLocal = matrixScale * matrixRotation * matrixTranslation;
	matrixWorld = matrixLocal;
	shared_ptr<Transform> myParent = GetParent().lock();
						 
	if (myParent != nullptr)
	{
		matrixWorld *= myParent->GetLocalToWorldMatrix();
	}
}

void Transform::PushDate()
{
	//TransformParams 구조체 초기화
	TransformParams transformParams = {};

	//월드 행렬 설정
	transformParams.matWorld = matrixWorld;

	//카메라의 뷰 행렬 설정(Camera 클라스의 정적 변수 사용)
	transformParams.matView = Camera::StaticMatrixView;
	//카메라의 투영 행렬 설정(Camera 클라스의 정적 변수 사용)
	transformParams.matProjection = Camera::StaticMatrixProjection;

	//월드-뷰 행렬 계산 및 설정(월드 행렬 * 뷰 행렬)
	transformParams.matWV = matrixWorld * Camera::StaticMatrixView;
	//월드-뷰-프로젝션 행렬 계산 및 설정(월드 행렬 * 뷰 행렬 * 프로젝션 행렬)
	transformParams.matWVP = matrixWorld * Camera::StaticMatrixView * Camera::StaticMatrixProjection;

	//상수 버퍼에 변환 매트릭스 데이터를 푸쉬
	Engine::Get().GetConstantBuffer(CONSTANT_BUFFER_TYPE::TRANSFORM)->PushData(&transformParams, sizeof(transformParams));
}
