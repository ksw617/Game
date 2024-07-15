#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Engine.h"
#include "Material.h"
#include "GameObject.h" 
#include "MeshFilter.h"


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
	
	//현재 씬의 마지막 Update 호출
	currentScene->FinalUpdate();
}


shared_ptr<Scene> SceneManager::LoadSampleScene()
{
	shared_ptr<Scene> testScene = make_shared<Scene>();//Test Scene 만들기

	shared_ptr<GameObject> gameObject = make_shared<GameObject>();

	vector<Vertex> vec(4);
	//Vector로 변환
	vec[0].pos = Vector3(-0.5f, 0.5f, 0.5f);
	vec[0].color = Vector4(1.f, 0.f, 0.f, 1.f);
	vec[0].uv = Vector2(0.f, 0.f);
	vec[1].pos = Vector3(0.5f, 0.5f, 0.5f);
	vec[1].color = Vector4(0.f, 1.f, 0.f, 1.f);
	vec[1].uv = Vector2(1.f, 0.f);
	vec[2].pos = Vector3(0.5f, -0.5f, 0.5f);
	vec[2].color = Vector4(0.f, 0.f, 1.f, 1.f);
	vec[2].uv = Vector2(1.f, 1.f);
	vec[3].pos = Vector3(-0.5f, -0.5f, 0.5f);
	vec[3].color = Vector4(0.f, 1.f, 0.f, 1.f);
	vec[3].uv = Vector2(0.f, 1.f);

	vector<UINT32> indexVertex;
	indexVertex.push_back(0);
	indexVertex.push_back(1);
	indexVertex.push_back(2);

	indexVertex.push_back(0);
	indexVertex.push_back(2);
	indexVertex.push_back(3);

	gameObject->Init();

	shared_ptr<MeshFilter> meshFilter = make_shared<MeshFilter>();

	{
		shared_ptr<Mesh> mesh = make_shared<Mesh>();
		mesh->Init(vec, indexVertex);

		meshFilter->SetMesh(mesh);
	}

	{
		shared_ptr<Shader> shader = make_shared<Shader>();
		shared_ptr<Texture> texture = make_shared<Texture>();

		shader->Init(L"..\\Resources\\Shader\\Default.hlsli");
		texture->Init(L"..\\Resources\\Texture\\DirectX_Image.png");

		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);
		material->SetFloat(0, 0.3f);
		material->SetFloat(1, 0.4f);
		material->SetFloat(2, 0.3f);
		material->SetTexture(0, texture);

		meshFilter->SetMaterial(material);

	}

	gameObject->AddComponent(meshFilter);

	//TestScene에 gameObject 추가
	testScene->AddGameObject(gameObject);

	return testScene;
}

