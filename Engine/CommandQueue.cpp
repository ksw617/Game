#include "pch.h"
#include "CommandQueue.h"

//Command Queue 클라스의 소멸자
CommandQueue::~CommandQueue()
{
	//팬스 이벤트 핸들을 닫아서 리소스를 해제
	CloseHandle(fenceEvent);
}

//초기화
void CommandQueue::Init(ComPtr<ID3D12Device> device, shared_ptr<class SwapChain> swapChain)
{
	//swapChain을 초기화
	this->swapChain = swapChain;

	//커맨드 큐 description 구조체 초기화
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT; //커맨드 리스트 타입 설정
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE; // 플래그 설정

	//커맨드 큐 생성
	device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&cmdQuene));

	//커맨드 allocator 생성
	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmdAlloc));

	//커맨드 리스트 생성
	device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAlloc.Get(), nullptr, IID_PPV_ARGS(&cmdList));

	//커맨드 리스트를 닫음, 재성정을 위해
	cmdList->Close();

	//팬스 객체 생성
	device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));


	//팬스 이밴트 생성
	//fenceEvent->CreateEvent(nullptr, FALSE, FALSE, nullptr);

	//내일 수정할께요.

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
