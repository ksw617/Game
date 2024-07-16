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
	//현재 씬을 가져옴
	shared_ptr<Scene> scene = SceneManager::Get().GetCurrentScene();

	//현재 씬의 모든 게임 객체를 가져옴
	const vector<shared_ptr<GameObject>>& gameObjects = scene->GetGameObjects();
									   
	//각 gameObject에 대한 처리
	for (auto& gameObject : gameObjects)
	{
		//gameObject에 MeshFilter가 없는 경우 건너띔
		if (gameObject->GetMeshFilter() == nullptr)
			continue;

		//gameObject의 MeshFilter 컴포넌트 Render를 실행
		gameObject->GetMeshFilter()->Render();

	}
}
