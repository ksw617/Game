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
	//조명데이터 푸쉬
	PushLightData();

	//SceneManager에서 해줬던거 여기서
	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetCamera() == nullptr)
			continue;

		gameObject->GetCamera()->Render();
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
