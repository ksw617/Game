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

	
				  
	viewPort = { 0,0, static_cast<FLOAT>(width), static_cast<FLOAT>(height), 0.0f, 1.0f };

	scissorRect = CD3DX12_RECT(0, 0, width, height);

	device = make_shared<Device>();
	cmdQueue = make_shared<CommandQueue>();
	swapChain = make_shared<SwapChain>();
	rootSignature = make_shared<RootSignature>();
	//constBuffer = make_shared<ConstantBuffer>();
	tableDesc = make_shared<TableDescriptor>();
	depthStencilBuffer = make_shared<DepthStencilBuffer>(); 

	input = make_shared<Input>(); 
	timer = make_shared<Timer>();

	device->Init();
	cmdQueue->Init(device->GetDevice(), swapChain);
	swapChain->Init(hwnd, width, height, windowed, device->GetDevice(), device->GetDXGI(), cmdQueue->GetCmdQueue());
	rootSignature->Init(device->GetDevice());
	//constBuffer->Init(CBV_REGISTER::b0, sizeof(XMFLOAT4), 256);

	//CreateConstantBuffer 호출
	CreateConstantBuffer(CBV_REGISTER::b0, sizeof(XMFLOAT4), 256);

	tableDesc->Init(256);


	input->Init(hwnd);
	timer->Init();

	ResizeWindow(width, height);
}

void Engine::Render()
{
	RenderBegin();

	RenderEnd();
}

void Engine::Update()
{
	input->Update();
	timer->Update();
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
	//reg값을 UINT8 타입으로 변환
	UINT8 typeInt = static_cast<UINT8>(reg);

	//constantBuffers 벡터의 크기와 typeInt 값이 같은지 확인 (디버그 검증)
	assert(constantBuffers.size() == typeInt);

	//새로운 ConstantBuffer 객체를 만들어 주고 초기화
	shared_ptr<ConstantBuffer> buffer = make_shared<ConstantBuffer>();
	buffer->Init(reg, bufferSize, count);

	//constantBuffers 벡터에 생성한 상수 버퍼를 추가
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
