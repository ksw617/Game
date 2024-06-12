#include "pch.h"
#include "Engine.h"
#include "Device.h"
#include "CommandQueue.h"
#include "SwapChain.h"

//���� �ʱ�ȭ �Լ� : ������ �ڵ�, ȭ�� �ʺ�, ����, â ��� ���θ� ���ڷ� ����
void Engine::Init(HWND _hwnd, int _width, int _height, bool _windowed)
{
	//���ڷ� ���� ���� ��� ������ ����
	hwnd = _hwnd;
	width = _width;
	height = _height;
	windowed = _windowed;

	//������ ũ�� ����
	ResizeWindow(_width, _height);

	//�׷��� ȭ���� ����Ʈ ����					  
	viewPort = { 0,0, static_cast<FLOAT>(width), static_cast<FLOAT>(height), 0.0f, 1.0f };

	//����(Rect) ����
	scissorRect = CD3DX12_RECT(0, 0, width, height);

	//Device, CommandQueue, SwapChain ��ü�� ����Ʈ �����ͷ� ����
	device = make_shared<Device>();
	cmdQueue = make_shared<CommandQueue>();
	swapChain = make_shared<SwapChain>();

	//Device �ʱ�ȭ
	device->Init();

	//CommandQueue �ʱ�ȭ(Device, SwapChain) ����
	cmdQueue->Init(device->GetDevice(), swapChain);

	//SwapChain �ʱ�ȭ(������ �ڵ�, �ʺ�, ����, â ���, Device, DXGI ���丮, CommmandQueue)
	swapChain->Init(hwnd, width, height, windowed, device->GetDevice(), device->GetDXGI(), cmdQueue->GetCmdQueue());
	
}

//������ �Լ� : ������ ���۰� ���� ����
void Engine::Render()
{
	//������ ����
	RederBegin();

	//Todo : ������ ��ü �׷��ֱ�
	//�߰����� ������ ������ ���⿡ �ۼ�

	//������ ����
	RenderEnd();
}

//������ ���� �Լ� : �������� �ʿ��� �ʱ� ������ ����
void Engine::RederBegin()
{
	//CommandQueue�� RenderBegin ȣ��(����Ʈ, ���� ����)
	cmdQueue->RenderBegin(&viewPort, &scissorRect);
}

//������ ���� �Լ� : ������ �� �ʿ��� ������ �۾��� ����
void Engine::RenderEnd()
{
	//CommandQueue�� RenderEnd ȣ��
	cmdQueue->RenderEnd();
}

void Engine::ResizeWindow(int _width, int _height)
{
	//���ο� �ʺ�� ���̸� �ɹ� ������ ����
	width = _width;
	height = _height;

	//������ ũ�⸦ �����ϱ� ���� RECT ����ü ����
	RECT rect = { 0, 0, width, height };

	//������ ũ�� ����
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	//������ ��ġ�� ũ��
	SetWindowPos(hwnd, 0, 100, 100, width, height, 0);
}
