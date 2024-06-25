#pragma once
class CommandQueue
{
private:
	ComPtr<ID3D12CommandQueue> cmdQuene;
	ComPtr<ID3D12CommandAllocator> cmdAlloc;
	ComPtr<ID3D12GraphicsCommandList> cmdList;

	ComPtr<ID3D12CommandAllocator> resCmdAlloc;	// ���ҽ� ��� �Ҵ���
	ComPtr<ID3D12GraphicsCommandList> resCmdList;// ���ҽ� ��� ����Ʈ 

	ComPtr<ID3D12Fence> fence;
	UINT32 fenceValue = 0;
	HANDLE fenceEvent = INVALID_HANDLE_VALUE;

	shared_ptr<class SwapChain> swapChain;

public:
	ComPtr<ID3D12CommandQueue> GetCmdQueue() { return cmdQuene; }
	ComPtr<ID3D12GraphicsCommandList> GetCmdList() { return cmdList; }
	//���ҽ� ��� ����Ʈ�� ��ȯ�ϴ� �Լ�
	ComPtr<ID3D12GraphicsCommandList> GetResourceCmdList() { return resCmdList; }

public:
	~CommandQueue();
public:
	void Init(ComPtr<ID3D12Device> device, shared_ptr<class SwapChain> _swapChain);
	void WaitSync();
	void RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect);
	void RenderEnd();

	//���ҽ� ��� ��⿭�� Submit�ϴ� �Լ�
	void SubmitResourceCommandQueue();


};

