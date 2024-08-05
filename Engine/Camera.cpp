#include "pch.h"
#include "Camera.h"
#include "Engine.h"
#include "Transform.h"
#include "Scene.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "MeshFilter.h"
#include "Material.h"  // �߰�

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
	// ���� �� ��������
	shared_ptr<Scene> scene = SceneManager::Get().GetCurrentScene();
	const vector<shared_ptr<GameObject>>& gameObjects = scene->GetGameObjects();

	//Deferred�� Forward�� Ŭ����
	deferredObjs.clear();
	forwardObjs.clear();

	// ���� ������Ʈ���� ��ȸ�ϸ� ����
	for (auto& gameObject : gameObjects)
	{
		// MeshFilter�� ������ ��ŵ
		if (gameObject->GetMeshFilter() == nullptr)
			continue;

		//�ø� ����ũ�� �ش� ���̾ �ش� �ȵǸ� ���
		if (!IsCulled(gameObject->GetLayerIndex()))
			continue;

		//�� �������� �ø�
		if (gameObject->GetCullingMask())
		{
			Vector3 center = gameObject->GetTransform()->GetWorldPosition();
			float radius = gameObject->GetTransform()->GetBoundingSphereRadius();

			if (!viewFrustum.ContainsSphere(center, radius))
				continue;
		}

		//�ش� ���� ������Ʈ ���̴� Ÿ�� �޾ƿ���
		SHADER_TYPE shaderType = gameObject->GetMeshFilter()->GetMaterial()->GetShader()->GetShaderType();
		switch (shaderType)
		{
			//���̴� Ÿ���� DEFERRED ���
			case SHADER_TYPE::DEFERRED:
				//deferredObjs�� �ش� gameObject �־��ֱ�
				deferredObjs.push_back(gameObject);
				break;
				//���̴� Ÿ���� FORWARD ���
			case SHADER_TYPE::FORWARD:
				//forwardObjs�� �ش� gameObject �־��ֱ�
				forwardObjs.push_back(gameObject);
				break;
		default:
			break;
		}
	}

	
}

void Camera::RenderDeferred()
{
	// ���� �� �� ���� ��� ����
	StaticMatrixView = matrixView;
	StaticMatrixProjection = matrixProjection;

	//deferredObjs ����
	for (auto& gameObject : deferredObjs)
	{

		gameObject->GetMeshFilter()->Render();

	}
}

void Camera::RenderForward()
{
	// ���� �� �� ���� ��� ����
	StaticMatrixView = matrixView;
	StaticMatrixProjection = matrixProjection;

	//forwardObjs ����
	for (auto& gameObject : forwardObjs)
	{

		gameObject->GetMeshFilter()->Render();

	}
}

//����
//void Camera::Render()
//{
//}
