#pragma once
class CommandQueue
{
private:
	ComPtr<ID3D12CommandQueue> cmdQuene;
	ComPtr<ID3D12CommandAllocator> cmdAlloc;
	ComPtr<ID3D12GraphicsCommandList> cmdList;

	ComPtr<ID3D12CommandAllocator> resCmdAlloc;	// 리소스 명령 할당자
	ComPtr<ID3D12GraphicsCommandList> resCmdList;// 리소스 명령 리스트 

	ComPtr<ID3D12Fence> fence;
	UINT32 fenceValue = 0;
	HANDLE fenceEvent = INVALID_HANDLE_VALUE;

	shared_ptr<class SwapChain> swapChain;

public:
	ComPtr<ID3D12CommandQueue> GetCmdQueue() { return cmdQuene; }
	ComPtr<ID3D12GraphicsCommandList> GetCmdList() { return cmdList; }
	//리소스 명령 리스트를 반환하는 함수
	ComPtr<ID3D12GraphicsCommandList> GetResourceCmdList() { return resCmdList; }

public:
	~CommandQueue();
public:
	void Init(ComPtr<ID3D12Device> device, shared_ptr<class SwapChain> _swapChain);
	void WaitSync();
	void RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect);
	void RenderEnd();

	//리소스 명령 대기열을 Submit하는 함수
	void SubmitResourceCommandQueue();


};

