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

		camera->SetName(L"Main_Camera");

		camera->AddComponent(make_shared<Transform>());
		camera->AddComponent(make_shared<Camera>());
		camera->AddComponent(make_shared<CameraMoveTest>());

		camera->GetTransform()->SetLocalPosition(Vector3(0.f, 0.f, 0.f));

		UINT8 layerIndex = LayerNameToIndex(L"UI");
		camera->GetCamera()->SetCullingMaskAll();
		camera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, false);	

		scene->AddGameObject(camera);
	}
#pragma endregion

#pragma region UI Camera
	{
		
		shared_ptr<GameObject> camera = make_shared<GameObject>();
		camera->SetName(L"UI_Camera");
		
		camera->AddComponent(make_shared<Transform>());
		camera->AddComponent(make_shared<Camera>());
		camera->GetTransform()->SetLocalPosition(Vector3(0.f, 0.f, 0.f));
		camera->GetCamera()->SetProjectionType(PROJECTION_TYPE::ORTHOGRAPHIC);
		
		UINT8 layerIndex = LayerNameToIndex(L"UI");
		camera->GetCamera()->ClearCullingMask();

		camera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, true);
		
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
			shared_ptr<Shader> shader = Resources::Get().GetResource<Shader>(L"Skybox");
			shared_ptr<Texture> texture = Resources::Get().Load<Texture>(L"SkyBox01", L"..\\Resources\\Texture\\skyBoxImage1.jpg");

			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
	
			meshFilter->SetMaterial(material);
		}
	
		skyBox->AddComponent(meshFilter);
	
		scene->AddGameObject(skyBox);
	
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
			//Deffered로 수정
			shared_ptr<Shader> shader = Resources::Get().GetResource<Shader>(L"Deffered");
			shared_ptr<Texture> texture = Resources::Get().Load<Texture>(L"Wood", L"..\\Resources\\Texture\\Stylized_Wood_Planks_002_basecolor.png");
			shared_ptr<Texture> normalMap = Resources::Get().Load<Texture>(L"Wood_Normal", L"..\\Resources\\Texture\\Stylized_Wood_Planks_002_normal.png");

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

#pragma region UI Plane
	for (int i = 0; i < 3; i++)
	{
		shared_ptr<GameObject> plane = make_shared<GameObject>();
		plane->SetLayerIndex(LayerNameToIndex(L"UI"));
		plane->AddComponent(make_shared<Transform>());

		plane->GetTransform()->SetLocalScale(Vector3(100.f, 100.f, 100.f));
		//위치값 수정
		plane->GetTransform()->SetLocalPosition(Vector3(-350.f + i * 160.f, 250.f, 500.f));

		shared_ptr<MeshFilter> meshFilter = make_shared<MeshFilter>();
		{
			shared_ptr<Mesh> mesh = Resources::Get().LoadRectangleMesh();
			meshFilter->SetMesh(mesh);
		}

		{

			//Forward로 수정
			shared_ptr<Shader> shader = Resources::Get().GetResource<Shader>(L"Forward");
			shared_ptr<Texture> texture = Engine::Get().GetMRT(RENDER_TARGET_TYPE::BUFFER)->GetRTTexture(i);
			//shared_ptr<Texture> texture = Resources::Get().Load<Texture>(L"Wood", L"..\\Resources\\Texture\\Stylized_Wood_Planks_002_basecolor.png");

			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);

			meshFilter->SetMaterial(material);
		}

		plane->AddComponent(meshFilter);
		scene->AddGameObject(plane);
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
