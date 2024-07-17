#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Engine.h"
#include "Material.h"
#include "GameObject.h" 
#include "MeshFilter.h"

#include "Camera.h"	
#include "Transform.h" 
#include "CameraMoveTest.h" 
#include "Resources.h" // 포함


void SceneManager::Render()
{
	if (currentScene == nullptr)
		return;

	const vector<shared_ptr<GameObject>>& gameObjects = currentScene->GetGameObjects();

	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetCamera() == nullptr)
			continue;

		gameObject->GetCamera()->Render();
	}

}

void SceneManager::LoadScene(wstring sceneName)
{
	currentScene = LoadSampleScene();

	currentScene->Awake();
	currentScene->Start();
}



void SceneManager::Update()
{

	if (currentScene == nullptr)
		return;

	currentScene->Update();
	currentScene->LateUpdate();

	currentScene->FinalUpdate();
}


shared_ptr<Scene> SceneManager::LoadSampleScene()
{
	shared_ptr<Scene> scene = make_shared<Scene>();

#pragma region Cube
	{
		//Cube 오브젝트 생성
		shared_ptr<GameObject> cube = make_shared<GameObject>();
		cube->Init();

		shared_ptr<Transform> transform = cube->GetTransform();

		transform->SetLocalPosition(Vector3(150.f, 100.f, 200.f));	// 위치값
		transform->SetLocalScale(Vector3(100.f, 100.f, 100.f));		// 크기값

		shared_ptr<MeshFilter> meshFilter = make_shared<MeshFilter>(); // MeshFilter 컴포넌트 생성

		{
			//Cube Mesh 로드 및 생성
			shared_ptr<Mesh> mesh = Resources::Get().LoadCubeMesh();
			meshFilter->SetMesh(mesh);
		}
		{
			//Shader 및 Texture 로드
			shared_ptr<Shader> shader = make_shared<Shader>();
			shared_ptr<Texture> texture = make_shared<Texture>();

			shader->Init(L"..\\Resources\\Shader\\Default.hlsli");
			texture->Init(L"..\\Resources\\Texture\\DirectX_Image.png");

			//Material 생성 및 설정
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);		   //쉐이더 설정
			material->SetTexture(0, texture);	   //텍스처 설정
			meshFilter->SetMaterial(material);	   //MeshFilter에 Material 설정
		}

		//Cube 오브젝트에 MeshFilter 컴포넌트 추가
		cube->AddComponent(meshFilter);
		//Scene에 Cube 오브젝트 추가
		scene->AddGameObject(cube);

	}
#pragma endregion

#pragma region Sphere
	{
		//sphere 오브젝트 생성
		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->Init();

		shared_ptr<Transform> transform = sphere->GetTransform();

		transform->SetLocalPosition(Vector3(0.f, 100.f, 200.f));	// 위치값
		transform->SetLocalScale(Vector3(100.f, 100.f, 100.f));		// 크기값

		shared_ptr<MeshFilter> meshFilter = make_shared<MeshFilter>(); // MeshFilter 컴포넌트 생성

		{
			//Cube Mesh 로드 및 생성
			shared_ptr<Mesh> mesh = Resources::Get().LoadSphereMesh();
			meshFilter->SetMesh(mesh);
		}
		{
			//Shader 및 Texture 로드
			shared_ptr<Shader> shader = make_shared<Shader>();
			shared_ptr<Texture> texture = make_shared<Texture>();

			shader->Init(L"..\\Resources\\Shader\\Default.hlsli");
			texture->Init(L"..\\Resources\\Texture\\DirectX_Image.png");

			//Material 생성 및 설정
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);		   //쉐이더 설정
			material->SetTexture(0, texture);	   //텍스처 설정
			meshFilter->SetMaterial(material);	   //MeshFilter에 Material 설정
		}

		//Cube 오브젝트에 MeshFilter 컴포넌트 추가
		sphere->AddComponent(meshFilter);
		//Scene에 Cube 오브젝트 추가
		scene->AddGameObject(sphere);
	}
#pragma endregion


#pragma region Camera
	{
		shared_ptr<GameObject> camera = make_shared<GameObject>();
		camera->AddComponent(make_shared<Transform>());
		camera->AddComponent(make_shared<Camera>());
		//카메라 움직여주는 커스텀 컴포넌트를 카메라에 붙임
		camera->AddComponent(make_shared<CameraMoveTest>());

		camera->GetTransform()->SetLocalPosition(Vector3(0.f, 100.f, 0.f));


		scene->AddGameObject(camera);
	}
#pragma endregion


	return scene;
}

