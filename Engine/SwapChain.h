#pragma once
class SwapChain
{
private:
	ComPtr<IDXGISwapChain> swapChain;
	
	//ComPtr<ID3D12Resource> rtvBuffer[2];
	//ComPtr<ID3D12DescriptorHeap> rtvHeap;
	//D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle[2];
	
	//수정
	UINT8 backBufferIndex = 0;

public:
	ComPtr<IDXGISwapChain> GetSwapChain() { return swapChain; }
	//backBufferIndex Get함수
	UINT8 GetBackBufferIndex() const { return backBufferIndex; }

	//ComPtr<ID3D12Resource> GetRenderTarget(int index) { return rtvBuffer[index]; }
	//ComPtr<ID3D12Resource> GetBackRTVBuffer() const { return rtvBuffer[backBufferIndex]; }
	//D3D12_CPU_DESCRIPTOR_HANDLE GetBackRTV() const { return rtvHandle[backBufferIndex]; }

public:
	void Init(HWND hwnd, int width, int height, bool windowed, ComPtr<ID3D12Device> device, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue);
	void Present();
	void SwapIndex();

private:
	void CreateSwapChain(HWND hwnd, int width, int height, bool windowed, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue);
	
	//void CreateRTV(ComPtr<ID3D12Device> device);
};

