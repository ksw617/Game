#include "pch.h"
#include "Engine.h"
#include "Material.h" 
#include "SceneManager.h"
#include "Light.h" 
#include "Resources.h"

void Engine::Init(HWND _hwnd, int _width, int _height, bool _windowed)
{
	hwnd = _hwnd;
	width = _width;
	height = _height;
	windowed = _windowed;

				  
	viewPort = { 0,0, static_cast<FLOAT>(width), static_cast<FLOAT>(height), 0.0f, 1.0f };

	scissorRect = CD3DX12_RECT(0, 0, width, height);

	device = make_shared<Device>();
	cmdQueue = make_shared<CommandQueue>();
	swapChain = make_shared<SwapChain>();
	rootSignature = make_shared<RootSignature>();
	tableDesc = make_shared<TableDescriptor>();
	depthStencilBuffer = make_shared<DepthStencilBuffer>(); 

	input = make_shared<Input>(); 
	timer = make_shared<Timer>();

	device->Init();
	cmdQueue->Init(device->GetDevice(), swapChain);
	swapChain->Init(hwnd, width, height, windowed, device->GetDevice(), device->GetDXGI(), cmdQueue->GetCmdQueue());
	rootSignature->Init(device->GetDevice());

	CreateConstantBuffer(CBV_REGISTER::b0, sizeof(LightParams), 1);
	CreateConstantBuffer(CBV_REGISTER::b1, sizeof(TransformParams), 256);
	CreateConstantBuffer(CBV_REGISTER::b2, sizeof(MaterialParams), 256);

	tableDesc->Init(256);


	input->Init(hwnd);
	timer->Init();

	//리소스 초기화
	Resources::Get().Init();

	ResizeWindow(width, height);
}

void Engine::Render()
{
	RenderBegin();

	SceneManager::Get().Render(); 

	RenderEnd();
}

void Engine::Update()
{
	input->Update();
	timer->Update();

	SceneManager::Get().Update();

	Render();

	ShowFPS();
}

void Engine::ShowFPS()
{
	UINT32 fps = timer->GetFPS();
	wstring fpsText = L"FPS: " + to_wstring(fps);

	SetWindowText(hwnd, fpsText.c_str());
}

void Engine::CreateConstantBuffer(CBV_REGISTER reg, UINT32 bufferSize, UINT32 count)
{
	UINT8 typeInt = static_cast<UINT8>(reg);

	assert(constantBuffers.size() == typeInt);

	shared_ptr<ConstantBuffer> buffer = make_shared<ConstantBuffer>();
	buffer->Init(reg, bufferSize, count);

	constantBuffers.push_back(buffer);

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

	depthStencilBuffer->Init(width, height);
}
