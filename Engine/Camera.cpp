#include "pch.h"
#include "Camera.h"
#include "Engine.h"
#include "Transform.h"
#include "Scene.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "MeshFilter.h"


Matrix Camera::StaticMatrixView;
Matrix Camera::StaticMatrixProjection;

Camera::Camera() : Component(COMPONENT_TYPE::CAMERA)
{
}

Camera::~Camera()
{
}

void Camera::FinalUpdate()
{
	matrixView = GetTransform()->GetLocalToWorldMatrix().Invert();

	float width = static_cast<float>(Engine::Get().GetWidth());
	float height = static_cast<float>(Engine::Get().GetHeight());

	if (type == PROJECTION_TYPE::PERSPECTIVE)
		matrixProjection = XMMatrixPerspectiveFovLH(fov, width / height, nearView, farView);
	else
		matrixProjection = XMMatrixOrthographicLH(width * size, height * size, nearView, farView);

	StaticMatrixView = matrixView;
	StaticMatrixProjection = matrixProjection;
}

void Camera::Render()
{
	//���� ���� ������
	shared_ptr<Scene> scene = SceneManager::Get().GetCurrentScene();

	//���� ���� ��� ���� ��ü�� ������
	const vector<shared_ptr<GameObject>>& gameObjects = scene->GetGameObjects();
									   
	//�� gameObject�� ���� ó��
	for (auto& gameObject : gameObjects)
	{
		//gameObject�� MeshFilter�� ���� ��� �ǳʶ�
		if (gameObject->GetMeshFilter() == nullptr)
			continue;

		//gameObject�� MeshFilter ������Ʈ Render�� ����
		gameObject->GetMeshFilter()->Render();

	}
}
