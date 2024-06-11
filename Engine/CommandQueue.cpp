#include "pch.h"
#include "CommandQueue.h"

//Command Queue Ŭ���� �Ҹ���
CommandQueue::~CommandQueue()
{
	//�ҽ� �̺�Ʈ �ڵ��� �ݾƼ� ���ҽ��� ����
	CloseHandle(fenceEvent);
}

//�ʱ�ȭ
void CommandQueue::Init(ComPtr<ID3D12Device> device, shared_ptr<class SwapChain> swapChain)
{
	//swapChain�� �ʱ�ȭ
	this->swapChain = swapChain;

	//Ŀ�ǵ� ť description ����ü �ʱ�ȭ
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT; //Ŀ�ǵ� ����Ʈ Ÿ�� ����
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE; // �÷��� ����

	//Ŀ�ǵ� ť ����
	device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&cmdQuene));

	//Ŀ�ǵ� allocator ����
	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmdAlloc));

	//Ŀ�ǵ� ����Ʈ ����
	device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAlloc.Get(), nullptr, IID_PPV_ARGS(&cmdList));

	//Ŀ�ǵ� ����Ʈ�� ����, �缺���� ����
	cmdList->Close();

	//�ҽ� ��ü ����
	device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));


	//�ҽ� �̹�Ʈ ����
	//fenceEvent->CreateEvent(nullptr, FALSE, FALSE, nullptr);

	//���� �����Ҳ���.

}

void CommandQueue::WaitSync()
{
}

void CommandQueue::RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect)
{
}

void CommandQueue::RenderEnd()
{
}
