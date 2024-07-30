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
#include "Light.h" 


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

#pragma region Layer Mask

	SetLayerName(0, L"Default");
	SetLayerName(5, L"UI");

#pragma endregion



#pragma region Main Camera
	{
		shared_ptr<GameObject> camera = make_shared<GameObject>();

		//카메라 이름 설정
		camera->SetName(L"Main_Camera");

		camera->AddComponent(make_shared<Transform>());
		camera->AddComponent(make_shared<Camera>());
		camera->AddComponent(make_shared<CameraMoveTest>());

		camera->GetTransform()->SetLocalPosition(Vector3(0.f, 100.f, 0.f));

		//"UI" 레이어의 인덱스를 가져옴
		UINT8 layerIndex = LayerNameToIndex(L"UI");
		camera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, true);	//UI는 안찍음

		scene->AddGameObject(camera);
	}
#pragma endregion

#pragma region UI Camera
	{
		shared_ptr<GameObject> camera = make_shared<GameObject>();

		//카메라 이름 설정
		camera->SetName(L"UI_Camera");

		camera->AddComponent(make_shared<Transform>());
		camera->AddComponent(make_shared<Camera>());

		//카메라 위치
		camera->GetTransform()->SetLocalPosition(Vector3(0.f, 0.f, 0.f));

		//카메라의 투영 타입을 ORTHOGRAPHIC타입으로 설정
		camera->GetCamera()->SetProjectionType(PROJECTION_TYPE::ORTHOGRAPHIC);

		//"UI" 레이어의 인덱스를 가져옴
		UINT8 layerIndex = LayerNameToIndex(L"UI");
		//모든 컬링 마스크를 끄도록 설정
		camera->GetCamera()->ClearCullingMask();
		//"UI" 레이어만 찍도록 컬링 마스크 설정(UI레이어만 활성화)
		camera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, false);

		scene->AddGameObject(camera);
	}
#pragma endregion

#pragma region SkyBox
	
	
	{
		shared_ptr<GameObject> skyBox = make_shared<GameObject>();
		skyBox->AddComponent(make_shared<Transform>());
		skyBox->SetCullingMask(false);
	
		shared_ptr<MeshFilter> meshFilter = make_shared<MeshFilter>();
		{
			shared_ptr<Mesh> sphereMesh = Resources::Get().LoadSphereMesh();
			meshFilter->SetMesh(sphereMesh);
		}
	
		{
			shared_ptr<Shader> shader = make_shared<Shader>();
			shared_ptr<Texture> texture = make_shared<Texture>();
	
			shader->Init(L"..\\Resources\\Shader\\SkyBox.fx", { RASTERRIZER_TYPE::CULL_NONE, DEPTH_STENCIL_TYPE::LESS_EQUAL });
			texture->Init(L"..\\Resources\\Texture\\skyBoxImage1.jpg");
	
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
	
			meshFilter->SetMaterial(material);
		}
	
		skyBox->AddComponent(meshFilter);
	
		scene->AddGameObject(skyBox);
	
	}			

#pragma endregion



#pragma region Directional Light
	{
		shared_ptr<GameObject> light = make_shared<GameObject>();
		light->AddComponent(make_shared<Transform>());

		light->AddComponent(make_shared<Light>());
		light->GetLight()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
		light->GetLight()->SetLightDirection(Vector3(1.f, -1.f, 1.f));
		light->GetLight()->SetDiffuse(Vector3(0.7f, 0.7f, 0.7f));
		light->GetLight()->SetAmbient(Vector3(0.3f, 0.3f, 0.3f));
		light->GetLight()->SetSpecular(Vector3(0.3f, 0.3f, 0.3f));
		scene->AddGameObject(light);
	}
#pragma endregion


#pragma region Sphere
	{
		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->AddComponent(make_shared<Transform>());

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
			shared_ptr<Texture> normalMap = make_shared<Texture>();

			shader->Init(L"..\\Resources\\Shader\\Default.fx");
			texture->Init(L"..\\Resources\\Texture\\Stylized_Wood_Planks_002_basecolor.png");
			normalMap->Init(L"..\\Resources\\Texture\\Stylized_Wood_Planks_002_normal.png");

			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			material->SetTexture(1, normalMap);
			meshFilter->SetMaterial(material);
		}

		sphere->AddComponent(meshFilter);
		scene->AddGameObject(sphere);
	}
#pragma endregion
		 
	return scene;
}

void SceneManager::SetLayerName(UINT8 index, const wstring& name)
{
	const wstring& prevName = layerNames[index];
	layerIndex.erase(prevName);

	layerNames[index] = name;
	layerIndex[name] = index;
}

UINT8 SceneManager::LayerNameToIndex(const wstring& name)
{
	auto findIt = layerIndex.find(name);
	if (findIt == layerIndex.end())
		return 0;

	return findIt->second;
}
