#include "pch.h"
#include "Scene.h"
#include "GameObject.h"

//필요한거 불러오기
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
	//조명 파라미터 구조체 초기화
	LightParams lightParams = {};
									   
	//게임 오브젝트들을 순회하면서 조명을 가져옴
	for (auto& gameObject : gameObjects)
	{
		//게임 오브젝트에 조명이 없으면 계속 진행
		if (gameObject->GetLight() == nullptr)
			continue;

		//게임 오브젝트의 조명 컴포넌트의 조명의 정보를 받아옴
		const LightInfo& lightInfo = gameObject->GetLight()->GetLightInfo();

		//조명 파라미터 구조체에 조명 정보를 추가
		lightParams.lights[lightParams.lightCount] = lightInfo;

		//조명 갯수를 증가
		lightParams.lightCount++;
	}

	//상수 버퍼에 전역 조명 데이터를 푸쉬
	Engine::Get().GetConstantBuffer(CONSTANT_BUFFER_TYPE::GLOBAL)->PushGlobalData(&lightParams, sizeof(lightParams));
}


void Scene::Render()
{
	PushLightData();

	// SwapChain Group 초기화
	// 현재 백 버퍼 인덱스를 가져와 초기화
	UINT8 backIndex = Engine::Get().GetSwapChain()->GetBackBufferIndex();
	Engine::Get().GetMRT(RENDER_TARGET_TYPE::SWAP_CHAIN)->ClearRenderTargetView(backIndex);


	// Deferred Group 초기화
	Engine::Get().GetMRT(RENDER_TARGET_TYPE::BUFFER)->ClearRenderTargetView();

	for (auto& gameObject : gameObjects)
	{
		// 카메라 컴포넌트가 없는 오브젝트는 스킵
		if (gameObject->GetCamera() == nullptr)
			continue;

		//gameObject->GetCamera()->Render();

		// 게임 오브젝트를 정렬 (지연 렌더링과 전방 렌더링으로 분류)
		gameObject->GetCamera()->SortGameObject();

		// 지연 렌더링 OMSet (Output Merger State 설정)
		Engine::Get().GetMRT(RENDER_TARGET_TYPE::BUFFER)->OMSetRenderTargets();
		gameObject->GetCamera()->RenderDeferred();

		// 스와체인 OMSet (Output Merger State 설정)
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
