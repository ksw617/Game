#include "pch.h"
#include "MainGame.h"
#include <Engine.h>
#include <SceneManager.h>


void MainGame::Init(HWND hwnd, int width, int height, bool windowed)
{
	Engine::Get().Init(hwnd, width, height, windowed);

	SceneManager::Get().LoadScene(L"SampleScene");
}

void MainGame::Tick()
{

	Engine::Get().Update();
}
