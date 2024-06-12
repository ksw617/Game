#include "pch.h"
#include "Engine.h"
#include "Device.h"
#include "CommandQueue.h"
#include "SwapChain.h"

//엔진 초기화 함수 : 윈도우 핸들, 화면 너비, 높이, 창 모드 여부를 인자로 받음
void Engine::Init(HWND _hwnd, int _width, int _height, bool _windowed)
{
	//인자로 받은 값을 멤버 변수에 저장
	hwnd = _hwnd;
	width = _width;
	height = _height;
	windowed = _windowed;

	//윈도우 크기 조정
	ResizeWindow(_width, _height);

	//그려질 화면의 뷰포트 설정					  
	viewPort = { 0,0, static_cast<FLOAT>(width), static_cast<FLOAT>(height), 0.0f, 1.0f };

	//시저(Rect) 설정
	scissorRect = CD3DX12_RECT(0, 0, width, height);

	//Device, CommandQueue, SwapChain 객체를 스마트 포인터로 생성
	device = make_shared<Device>();
	cmdQueue = make_shared<CommandQueue>();
	swapChain = make_shared<SwapChain>();

	//Device 초기화
	device->Init();

	//CommandQueue 초기화(Device, SwapChain) 전달
	cmdQueue->Init(device->GetDevice(), swapChain);

	//SwapChain 초기화(윈도우 핸들, 너비, 높이, 창 모드, Device, DXGI 팩토리, CommmandQueue)
	swapChain->Init(hwnd, width, height, windowed, device->GetDevice(), device->GetDXGI(), cmdQueue->GetCmdQueue());
	
}

//랜더링 함수 : 랜더링 시작과 끝을 관리
void Engine::Render()
{
	//랜더링 시작
	RederBegin();

	//Todo : 나머지 물체 그려주기
	//추가적인 랜더링 로직을 여기에 작성

	//랜더링 종료
	RenderEnd();
}

//랜더링 시작 함수 : 랜더링에 필요한 초기 설정을 수행
void Engine::RederBegin()
{
	//CommandQueue의 RenderBegin 호출(뷰포트, 시저 전달)
	cmdQueue->RenderBegin(&viewPort, &scissorRect);
}

//랜더링 종료 함수 : 랜더링 후 필요한 마무리 작업을 수행
void Engine::RenderEnd()
{
	//CommandQueue의 RenderEnd 호출
	cmdQueue->RenderEnd();
}

void Engine::ResizeWindow(int _width, int _height)
{
	//새로운 너비와 높이를 맴버 변수에 저장
	width = _width;
	height = _height;

	//윈도우 크기를 조정하기 위한 RECT 구조체 설정
	RECT rect = { 0, 0, width, height };

	//윈도우 크기 조정
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	//윈도우 위치와 크기
	SetWindowPos(hwnd, 0, 100, 100, width, height, 0);
}
