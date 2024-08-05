#include "pch.h"
#include "Scene.h"
#include "GameObject.h"

//�ʿ��Ѱ� �ҷ�����
#include "Engine.h"
#include "ConstantBuffer.h"
#include "Camera.h"
#include "Light.h"

void Scene::Awake()
{
	for (const shared_ptr<GameObject>& gameObject : gameObjects)
	{
		gameObject->Awake();
	}
}

void Scene::Start()
{
	for (const shared_ptr<GameObject>& gameObject : gameObjects)
	{
		gameObject->Start();
	}
}

void Scene::Update()
{
	for (const shared_ptr<GameObject>& gameObject : gameObjects)
	{
		gameObject->Update();
	}
}

void Scene::LateUpdate()
{
	for (const shared_ptr<GameObject>& gameObject : gameObjects)
	{
		gameObject->LateUpdate();
	}
}

void Scene::FinalUpdate()
{	 	
	for (const shared_ptr<GameObject>& gameObject : gameObjects)
	{
		gameObject->FinalUpdate();
	}
}


void Scene::PushLightData()
{
	//���� �Ķ���� ����ü �ʱ�ȭ
	LightParams lightParams = {};
									   
	//���� ������Ʈ���� ��ȸ�ϸ鼭 ������ ������
	for (auto& gameObject : gameObjects)
	{
		//���� ������Ʈ�� ������ ������ ��� ����
		if (gameObject->GetLight() == nullptr)
			continue;

		//���� ������Ʈ�� ���� ������Ʈ�� ������ ������ �޾ƿ�
		const LightInfo& lightInfo = gameObject->GetLight()->GetLightInfo();

		//���� �Ķ���� ����ü�� ���� ������ �߰�
		lightParams.lights[lightParams.lightCount] = lightInfo;

		//���� ������ ����
		lightParams.lightCount++;
	}

	//��� ���ۿ� ���� ���� �����͸� Ǫ��
	Engine::Get().GetConstantBuffer(CONSTANT_BUFFER_TYPE::GLOBAL)->PushGlobalData(&lightParams, sizeof(lightParams));
}


void Scene::Render()
{
	PushLightData();

	// SwapChain Group �ʱ�ȭ
	// ���� �� ���� �ε����� ������ �ʱ�ȭ
	UINT8 backIndex = Engine::Get().GetSwapChain()->GetBackBufferIndex();
	Engine::Get().GetMRT(RENDER_TARGET_TYPE::SWAP_CHAIN)->ClearRenderTargetView(backIndex);


	// Deferred Group �ʱ�ȭ
	Engine::Get().GetMRT(RENDER_TARGET_TYPE::BUFFER)->ClearRenderTargetView();

	for (auto& gameObject : gameObjects)
	{
		// ī�޶� ������Ʈ�� ���� ������Ʈ�� ��ŵ
		if (gameObject->GetCamera() == nullptr)
			continue;

		//gameObject->GetCamera()->Render();

		// ���� ������Ʈ�� ���� (���� �������� ���� ���������� �з�)
		gameObject->GetCamera()->SortGameObject();

		// ���� ������ OMSet (Output Merger State ����)
		Engine::Get().GetMRT(RENDER_TARGET_TYPE::BUFFER)->OMSetRenderTargets();
		gameObject->GetCamera()->RenderDeferred();

		// ����ü�� OMSet (Output Merger State ����)
		Engine::Get().GetMRT(RENDER_TARGET_TYPE::SWAP_CHAIN)->OMSetRenderTargets(1, backIndex);
		gameObject->GetCamera()->RenderDeferred();
	}
}

void Scene::AddGameObject(shared_ptr<GameObject> gameObject)
{
	gameObjects.push_back(gameObject);
}

void Scene::RemoveGameObject(shared_ptr<GameObject> gameObject)
{		
	auto findIt = find(gameObjects.begin(), gameObjects.end(), gameObject);
	if (findIt != gameObjects.end())
	{
		gameObjects.erase(findIt);
	}
}
