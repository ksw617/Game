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
		shared_ptr<GameObject> light = make_shared<GameObject>();
		light->Init();
		light->AddComponent(make_shared<Light>());
		light->GetLight()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
		light->GetLight()->SetLightDirection(Vector3(1.f, -1.f, 1.f));
		light->GetLight()->SetDiffuse(Vector3(0.7f, 0.f, 0.f));
		light->GetLight()->SetAmbient(Vector3(0.3f, 0.f, 0.f));
		light->GetLight()->SetSpecular(Vector3(0.3f, 0.3f, 0.3f));
		scene->AddGameObject(light);
	}
#pragma endregion

#pragma region Point Light
	{
		shared_ptr<GameObject> light = make_shared<GameObject>();
		light->Init();
		light->AddComponent(make_shared<Light>());
		light->GetLight()->SetLightType(LIGHT_TYPE::POINT_LIGHT);
		light->GetTransform()->SetLocalPosition(Vector3(-150.f, 100.f, 200.f));
		light->GetLight()->SetDiffuse(Vector3(0.f, 0.7f, 0.f));
		light->GetLight()->SetAmbient(Vector3(0.1f, 0.1f, 0.1f));
		light->GetLight()->SetSpecular(Vector3(0.1f, 0.1f, 0.1f));

		light->GetLight()->SetLightRange(10000.f);

		scene->AddGameObject(light);
	}
#pragma endregion

#pragma region Spot Light
	{
		shared_ptr<GameObject> light = make_shared<GameObject>();
		light->Init();
		light->AddComponent(make_shared<Light>());
		//Spot Light 설정
		light->GetLight()->SetLightType(LIGHT_TYPE::SPOT_LIGHT);
		//위치 설정
		light->GetLight()->SetLightDirection(Vector3(-1.f, 0.f, 0.f));
		light->GetTransform()->SetLocalPosition(Vector3(150.f, 100.f, 200.f));
		light->GetLight()->SetDiffuse(Vector3(0.f, 0.f, 0.7f));
		light->GetLight()->SetAmbient(Vector3(0.1f, 0.1f, 0.1f));
		light->GetLight()->SetSpecular(Vector3(0.1f, 0.1f, 0.1f));

		light->GetLight()->SetLightRange(10000.f);
		//광원의 각도 설정
		light->GetLight()->SetLightAngle(XM_PI / 4);

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
