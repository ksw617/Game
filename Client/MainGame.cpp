#include "pch.h"
#include "MainGame.h"
#include <Engine.h>

shared_ptr<Mesh> mesh = make_shared<Mesh>();
shared_ptr<Shader> shader = make_shared<Shader>();

void MainGame::Init(HWND hwnd, int width, int height, bool windowed)
{
	Engine::Get().Init(hwnd, width, height, windowed);

	//정점 데이터 초기화(삼각형 정의)
	vector<Vertex> vec(3);
	vec[0].pos = XMFLOAT3(0.f, 0.5f, 0.5f); 
	vec[0].color = XMFLOAT4(1.f, 0.f, 0.f, 1.f); //빨강색 정점
	vec[1].pos = XMFLOAT3(0.5f, -0.5f, 0.5f);
	vec[1].color = XMFLOAT4(0.f, 1.f, 0.f, 1.f); //초록색 정점
	vec[2].pos = XMFLOAT3(-0.5f, -0.5f, 0.5f);
	vec[2].color = XMFLOAT4(0.f, 0.f, 1.f, 1.f); //파란색 정점

	//매쉬 초기화
	mesh->Init(vec);

	//쉐이더 초기화
	shader->Init(L"..\\Resources\\Shader\\Default.hlsli");

	//명령 대기열 동기화 대기
	Engine::Get().GetCmdQueue()->WaitSync();
	

}

void MainGame::Tick()
{
	//랜더링 시작
	Engine::Get().RenderBegin();

	//쉐이더 업데이트
	shader->Update();

	//메쉬 랜더링
	mesh->Render();

	//랜더링 종료
	Engine::Get().RenderEnd();
}
