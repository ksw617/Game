#include "pch.h"
#include "Camera.h"
#include "Engine.h"
#include "Transform.h"
#include "Scene.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "MeshFilter.h"
#include "Material.h"  // 추가

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

	viewFrustum.FinalUpdate();
}

void Camera::SortGameObject()
{
	// 현재 씬 가져오기
	shared_ptr<Scene> scene = SceneManager::Get().GetCurrentScene();
	const vector<shared_ptr<GameObject>>& gameObjects = scene->GetGameObjects();

	//Deferred용 Forward용 클리어
	deferredObjs.clear();
	forwardObjs.clear();

	// 게임 오브젝트들을 순회하며 정렬
	for (auto& gameObject : gameObjects)
	{
		// MeshFilter가 없으면 스킵
		if (gameObject->GetMeshFilter() == nullptr)
			continue;

		//컬링 마스크에 해당 레이어가 해당 안되면 통과
		if (!IsCulled(gameObject->GetLayerIndex()))
			continue;

		//뷰 프러스텀 컬링
		if (gameObject->GetCullingMask())
		{
			Vector3 center = gameObject->GetTransform()->GetWorldPosition();
			float radius = gameObject->GetTransform()->GetBoundingSphereRadius();

			if (!viewFrustum.ContainsSphere(center, radius))
				continue;
		}

		//해당 게임 오브젝트 쉐이더 타입 받아오기
		SHADER_TYPE shaderType = gameObject->GetMeshFilter()->GetMaterial()->GetShader()->GetShaderType();
		switch (shaderType)
		{
			//쉐이더 타입이 DEFERRED 경우
			case SHADER_TYPE::DEFERRED:
				//deferredObjs에 해당 gameObject 넣어주기
				deferredObjs.push_back(gameObject);
				break;
				//쉐이더 타입이 FORWARD 경우
			case SHADER_TYPE::FORWARD:
				//forwardObjs에 해당 gameObject 넣어주기
				forwardObjs.push_back(gameObject);
				break;
		default:
			break;
		}
	}

	
}

void Camera::RenderDeferred()
{
	// 정적 뷰 및 투영 행렬 설정
	StaticMatrixView = matrixView;
	StaticMatrixProjection = matrixProjection;

	//deferredObjs 랜더
	for (auto& gameObject : deferredObjs)
	{

		gameObject->GetMeshFilter()->Render();

	}
}

void Camera::RenderForward()
{
	// 정적 뷰 및 투영 행렬 설정
	StaticMatrixView = matrixView;
	StaticMatrixProjection = matrixProjection;

	//forwardObjs 랜더
	for (auto& gameObject : forwardObjs)
	{

		gameObject->GetMeshFilter()->Render();

	}
}

//삭제
//void Camera::Render()
//{
//}
