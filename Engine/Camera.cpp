#include "pch.h"
#include "Camera.h"
#include "Engine.h"
#include "Transform.h"

//���� ���� �ʱ�ȭ
Matrix Camera::StaticMatrixView;
Matrix Camera::StaticMatrixProjection;

//ī�޶� ������Ʈ ������ : ������Ʈ Ÿ���� CAMERA�� ����
Camera::Camera() : Component(COMPONENT_TYPE::CAMERA)
{
}

Camera::~Camera()
{
}

void Camera::FinalUpdate()
{
	//�� ��� ���: ��ȯ ������Ʈ�� ���� ���� ��ȯ ����� �����ͼ� ������� ����
	matrixView = GetTransform()->GetLocalToWorldMatrix().Invert();

	//ȭ���� �ʺ�� ���̸� float���� ������
	float width = static_cast<float>(Engine::Get().GetWidth());
	float height = static_cast<float>(Engine::Get().GetHeight());

	//ī�޶� Ÿ�Կ� ���� ���� ��� ���
	if (type == PROJECTION_TYPE::PERSPECTIVE)
		//���� ���� ����� ���
		matrixProjection = XMMatrixPerspectiveFovLH(fov, width / height, nearView, farView);
	else
		//���� ���� ����� ���
		matrixProjection = XMMatrixOrthographicLH(width * size, height * size, nearView, farView);

	//���� ������ �� ��İ� ���� ����� ����
	StaticMatrixView = matrixView;
	StaticMatrixProjection = matrixProjection;
}