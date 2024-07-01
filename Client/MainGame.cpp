#include "pch.h"
#include "MainGame.h"
#include <Engine.h>

shared_ptr<Mesh> mesh = make_shared<Mesh>();
shared_ptr<Shader> shader = make_shared<Shader>();
shared_ptr<Texture> texture = make_shared<Texture>();

void MainGame::Init(HWND hwnd, int width, int height, bool windowed)
{
	Engine::Get().Init(hwnd, width, height, windowed);


	vector<Vertex> vec(4);
	vec[0].pos = XMFLOAT3(-0.5f, 0.5f, 0.5f); 
	vec[0].color = XMFLOAT4(1.f, 0.f, 0.f, 1.f); 
	vec[0].uv = XMFLOAT2(0.f, 0.f);
	vec[1].pos = XMFLOAT3(0.5f, 0.5f, 0.5f);
	vec[1].color = XMFLOAT4(0.f, 1.f, 0.f, 1.f);
	vec[1].uv = XMFLOAT2(1.f, 0.f);
	vec[2].pos = XMFLOAT3(0.5f, -0.5f, 0.5f);
	vec[2].color = XMFLOAT4(0.f, 0.f, 1.f, 1.f);
	vec[2].uv = XMFLOAT2(1.f, 1.f);
	vec[3].pos = XMFLOAT3(-0.5f, -0.5f, 0.5f);
	vec[3].color = XMFLOAT4(0.f, 1.f, 0.f, 1.f);
	vec[3].uv = XMFLOAT2(0.f, 1.f);

	vector<UINT32> indexVertex;
	indexVertex.push_back(0);
	indexVertex.push_back(1);
	indexVertex.push_back(2);

	indexVertex.push_back(0);
	indexVertex.push_back(2); 	
	indexVertex.push_back(3);


	mesh->Init(vec, indexVertex);

	shader->Init(L"..\\Resources\\Shader\\Default.hlsli");
	texture->Init(L"..\\Resources\\Texture\\DirectX_Image.png");

	Engine::Get().GetCmdQueue()->WaitSync();
	

}

void MainGame::Tick()
{
	Engine::Get().RenderBegin();

	//엔진의 업데이트 함수 호출
	Engine::Get().Update();

	shader->Update();

	{
		static XMFLOAT4 transform = {};

		//Time값 가져오기
		float deltaTime = Engine::Get().GetTimer()->GetDeltaTime();

		if (Engine::Get().GetInput()->GetButton(KEY_TYPE::W))
		{
			transform.y += 1.f * deltaTime;
		}
		if (Engine::Get().GetInput()->GetButton(KEY_TYPE::S))
		{
			transform.y -= 1.f * deltaTime;
		}
		if (Engine::Get().GetInput()->GetButton(KEY_TYPE::A))
		{
			transform.x -= 1.f * deltaTime;
		}
		if (Engine::Get().GetInput()->GetButton(KEY_TYPE::D))
		{
			transform.x += 1.f * deltaTime;
		}

		mesh->SetTransform(transform);

		mesh->SetTexture(texture);

		mesh->Render();

	}


	Engine::Get().RenderEnd();
}
