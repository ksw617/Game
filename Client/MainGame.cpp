#include "pch.h"
#include "MainGame.h"
#include <Engine.h>

shared_ptr<Mesh> mesh = make_shared<Mesh>();
shared_ptr<Shader> shader = make_shared<Shader>();

void MainGame::Init(HWND hwnd, int width, int height, bool windowed)
{
	Engine::Get().Init(hwnd, width, height, windowed);

	vector<Vertex> vec(3);
	vec[0].pos = XMFLOAT3(0.f, 0.5f, 0.5f); 
	vec[0].color = XMFLOAT4(1.f, 0.f, 0.f, 1.f); 
	vec[1].pos = XMFLOAT3(0.5f, -0.5f, 0.5f);
	vec[1].color = XMFLOAT4(0.f, 1.f, 0.f, 1.f);
	vec[2].pos = XMFLOAT3(-0.5f, -0.5f, 0.5f);
	vec[2].color = XMFLOAT4(0.f, 0.f, 1.f, 1.f);

	mesh->Init(vec);

	shader->Init(L"..\\Resources\\Shader\\Default.hlsli");

	Engine::Get().GetCmdQueue()->WaitSync();
	

}

void MainGame::Tick()
{
	Engine::Get().RenderBegin();

	shader->Update();

	{
		XMFLOAT4 transform(0.2f, 0.f, 0.f, 0.f);
		mesh->SetTransform(transform);

		mesh->Render();

	}
	

	{
		XMFLOAT4 transform(0.f, 0.75f, 0.f, 0.f);
		mesh->SetTransform(transform);

		mesh->Render();
	}

	Engine::Get().RenderEnd();
}
