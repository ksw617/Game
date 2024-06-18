#include "pch.h"
#include "Engine.h"
#include "Device.h"
#include "CommandQueue.h"
#include "SwapChain.h"

void Engine::Init(HWND _hwnd, int _width, int _height, bool _windowed)
{
	hwnd = _hwnd;
	width = _width;
	height = _height;
	windowed = _windowed;

	ResizeWindow(_width, _height);
				  
	viewPort = { 0,0, static_cast<FLOAT>(width), static_cast<FLOAT>(height), 0.0f, 1.0f };

	scissorRect = CD3DX12_RECT(0, 0, width, height);

	device = make_shared<Device>();
	cmdQueue = make_shared<CommandQueue>();
	swapChain = make_shared<SwapChain>();
	rootSignature = make_shared<RootSignature>();
	//할당
	constBuffer = make_shared<ConstantBuffer>();

	device->Init();
	cmdQueue->Init(device->GetDevice(), swapChain);
	swapChain->Init(hwnd, width, height, windowed, device->GetDevice(), device->GetDXGI(), cmdQueue->GetCmdQueue());
	rootSignature->Init(device->GetDevice());
	//초기화
	constBuffer->Init(sizeof(XMFLOAT4), 256);

}

void Engine::Render()
{
	RenderBegin();

	RenderEnd();
}

void Engine::RenderBegin()
{
	cmdQueue->RenderBegin(&viewPort, &scissorRect);
}

void Engine::RenderEnd()
{
	cmdQueue->RenderEnd();
}

void Engine::ResizeWindow(int _width, int _height)
{
	width = _width;
	height = _height;

	RECT rect = { 0, 0, width, height };

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	SetWindowPos(hwnd, 0, 100, 100, width, height, 0);
}
