#include "pch.h"
#include "MainGame.h"
#include <Engine.h>

void MainGame::Init(HWND hwnd, int width, int height, bool windowed)
{
	Engine::Get().Init(hwnd, width, height, windowed);

}

void MainGame::Tick()
{
	Engine::Get().Render();
}
