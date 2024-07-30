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

	if (projectionType == PROJECTION_TYPE::PERSPECTIVE)
		matrixProjection = XMMatrixPerspectiveFovLH(fov, width / height, nearView, farView);
	else
		matrixProjection = XMMatrixOrthographicLH(width * size, height * size, nearView, farView);

	//StaticMatrixView = matrixView;
	//StaticMatrixProjection = matrixProjection;

	viewFrustum.FinalUpdate();
}

void Camera::Render()
{
	//Render 쪽으로 이동
	StaticMatrixView = matrixView;
	StaticMatrixProjection = matrixProjection;

	shared_ptr<Scene> scene = SceneManager::Get().GetCurrentScene();
	const vector<shared_ptr<GameObject>>& gameObjects = scene->GetGameObjects();

	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetMeshFilter() == nullptr)
			continue;

		//컬링 마스크에 해당 레이어가 해당 안되면 통과
		if (IsCulled(gameObject->GetLayerIndex()))
			continue;

		if (gameObject->GetCullingMask())
		{
			Vector3 center = gameObject->GetTransform()->GetWorldPosition();
			float radius = gameObject->GetTransform()->GetBoundingSphereRadius();

			if (!viewFrustum.ContainsSphere(center, radius))
				continue;
		}

		gameObject->GetMeshFilter()->Render();

	}
}
