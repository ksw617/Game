#include "pch.h"
#include "Scene.h"
#include "GameObject.h"

void Scene::Awake()
{
	//모든 GameObject의 Awake함수 호출
	for (const shared_ptr<GameObject>& gameObject : gameObjects)
	{
		gameObject->Awake();
	}
}

void Scene::Start()
{
	//모든 GameObject의 Start함수 호출
	for (const shared_ptr<GameObject>& gameObject : gameObjects)
	{
		gameObject->Start();
	}
}

void Scene::Update()
{
	//모든 GameObject의 Update함수 호출
	for (const shared_ptr<GameObject>& gameObject : gameObjects)
	{
		gameObject->Update();
	}
}

void Scene::LateUpdate()
{
	//모든 GameObject의 LateUpdate함수 호출
	for (const shared_ptr<GameObject>& gameObject : gameObjects)
	{
		gameObject->LateUpdate();
	}
}

void Scene::AddGameObject(shared_ptr<GameObject> gameObject)
{
	//gameObject를 gameObjects에 추가
	gameObjects.push_back(gameObject);
}

void Scene::RemoveGameObject(shared_ptr<GameObject> gameObject)
{		
	//gameObject를 gameObjects에서 제거
	auto findIt = find(gameObjects.begin(), gameObjects.end(), gameObject);
	if (findIt != gameObjects.end())
	{
		gameObjects.erase(findIt);
	}
}
