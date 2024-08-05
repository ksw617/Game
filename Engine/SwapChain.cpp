#include "pch.h"
#include "SwapChain.h"

void SwapChain::Init(HWND hwnd, int width, int height, bool windowed, ComPtr<ID3D12Device> device, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue)
{
	CreateSwapChain(hwnd, width, height, windowed, dxgi, cmdQueue);

	//CreateRTV(device);
}

void SwapChain::Present()
{
	swapChain->Present(0, 0);
}

void SwapChain::SwapIndex()
{
	backBufferIndex = (backBufferIndex + 1)	% 2;
}

void SwapChain::CreateSwapChain(HWND hwnd, int width, int height, bool windowed, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue)
{
	swapChain.Reset();
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = static_cast<unsigned int>(width);
	sd.BufferDesc.Height = static_cast<unsigned int>(height);

	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;

	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2;
	sd.OutputWindow = hwnd;
	sd.Windowed = windowed;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; 
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; 


	dxgi->CreateSwapChain(cmdQueue.Get(), &sd, &swapChain);

	//for (int i = 0; i < 2; i++)
	//{
	//	swapChain->GetBuffer(i, IID_PPV_ARGS(&rtvBuffer[i]));
	//}

}
/*
void SwapChain::CreateRTV(ComPtr<ID3D12Device> device)
{
	//RTV 디스크립터 힙의 크기 계산
	int rtvHeapSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	//RTV DESCRIPTOR HEAP DESC 구조체 초기화
	D3D12_DESCRIPTOR_HEAP_DESC rtvDesc;
	//디스크립트 힙 타입 설정
	rtvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	//디스크립터 수 설정(더블 버퍼링)
	rtvDesc.NumDescriptors = 2;
	//디스크립터 힙 플래그 설정
	rtvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	//노드 마스크 설정(단일 어댑터의 경우 0)
	rtvDesc.NodeMask = 0;

	//RTV 디스크립터 힙 생성
	device->CreateDescriptorHeap(&rtvDesc, IID_PPV_ARGS(&rtvHeap));

	//RTV 디스크립터 힙의 시작 핸들 가져오기
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHeapBegin = rtvHeap->GetCPUDescriptorHandleForHeapStart();

	//각 버퍼에 대한 RTV 생성
	for (int i = 0; i < 2; i++)
	{
		//RTV 핸들 설정
		rtvHandle[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeapBegin, i * rtvHeapSize);

		//RTV 생성
		device->CreateRenderTargetView(rtvBuffer[i].Get(), nullptr, rtvHandle[i]);
	}
}
 */