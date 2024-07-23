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
#include "Resources.h" 
#include "Light.h" // Light 추가


void SceneManager::Render()
{
	if (currentScene != nullptr)
		currentScene->Render();


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

#pragma region Sphere
	{
		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->Init();

		shared_ptr<Transform> transform = sphere->GetTransform();

		transform->SetLocalPosition(Vector3(0.f, 100.f, 200.f));
		transform->SetLocalScale(Vector3(100.f, 100.f, 100.f));

		shared_ptr<MeshFilter> meshFilter = make_shared<MeshFilter>();

		{

			shared_ptr<Mesh> mesh = Resources::Get().LoadSphereMesh();
			meshFilter->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = make_shared<Shader>();
			shared_ptr<Texture> texture = make_shared<Texture>();

			shader->Init(L"..\\Resources\\Shader\\Default.hlsli");
			texture->Init(L"..\\Resources\\Texture\\DirectX_Image.png");

			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshFilter->SetMaterial(material);
		}

		sphere->AddComponent(meshFilter);
		scene->AddGameObject(sphere);
	}
#pragma endregion

#pragma region Directional Light
	{
		//새로운 게임 오브젝트 생성
		shared_ptr<GameObject> light = make_shared<GameObject>();
		//게임 오브젝트 초기화
		light->Init();
		//해당 게임 오브젝트에 라이트 컴포넌트 추가
		light->AddComponent(make_shared<Light>());
		//라이트 타입 설정(DIRECTIONAL_LIGHT)
		light->GetLight()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
		//라이트의 방향 설정
		light->GetLight()->SetLightDirection(Vector3(0.f, -1.f, 0.f));
		//확산광 색상 설정(RGB)
		light->GetLight()->SetDiffuse(Vector3(0.5f, 0.5f, 0.5f));
		//주변 색상 설정(RGB)
		light->GetLight()->SetAmbient(Vector3(0.1f, 0.1f, 0.1f));
		//반사광 색상 설정(RGB)
		light->GetLight()->SetSpecular(Vector3(0.3f, 0.f, 0.3f));

		//씬에 해당 게임 오브젝트 추가
		scene->AddGameObject(light);
	}
#pragma endregion


#pragma region Camera
	{
		shared_ptr<GameObject> camera = make_shared<GameObject>();
		camera->AddComponent(make_shared<Transform>());
		camera->AddComponent(make_shared<Camera>());
		camera->AddComponent(make_shared<CameraMoveTest>());

		camera->GetTransform()->SetLocalPosition(Vector3(0.f, 100.f, 0.f));


		scene->AddGameObject(camera);
	}
#pragma endregion


	return scene;
}
