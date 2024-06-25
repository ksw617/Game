#include "pch.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "Engine.h"

CommandQueue::~CommandQueue()
{
	CloseHandle(fenceEvent);
}

void CommandQueue::Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> _swapChain)
{
	swapChain = _swapChain;

	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE; 

	device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&cmdQuene));
	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmdAlloc));
	device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAlloc.Get(), nullptr, IID_PPV_ARGS(&cmdList));
	cmdList->Close();
	
	//리소스 명령 할당자 생성
	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&resCmdAlloc));
	//리소스 명령 리스트를 생성
	device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, resCmdAlloc.Get(), nullptr, IID_PPV_ARGS(&resCmdList));


	device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));

	fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

}

void CommandQueue::WaitSync()
{
	fenceValue++;

	cmdQuene->Signal(fence.Get(), fenceValue);

	if (fence->GetCompletedValue() < fenceValue)
	{
		fence->SetEventOnCompletion(fenceValue, fenceEvent);

		WaitForSingleObject(fenceEvent, INFINITE);
	}

}

void CommandQueue::RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect)
{
	cmdAlloc->Reset();
	cmdList->Reset(cmdAlloc.Get(), nullptr);

	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		swapChain->GetBackRTVBuffer().Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	
	cmdList->SetGraphicsRootSignature(Engine::Get().GetRootSignature()->GetSignature().Get());
	
	Engine::Get().GetConstBuffer()->Clear();
	Engine::Get().GetTableDesc()->Clear();

	ID3D12DescriptorHeap* descHeap = Engine::Get().GetTableDesc()->GetDescriptorHeap().Get();
	cmdList->SetDescriptorHeaps(1, &descHeap);

	cmdList->ResourceBarrier(1, &barrier);
	cmdList->RSSetViewports(1, vp);
	cmdList->RSSetScissorRects(1, rect);


	D3D12_CPU_DESCRIPTOR_HANDLE backBufferView = swapChain->GetBackRTV();

	cmdList->ClearRenderTargetView(backBufferView, Colors::Aqua, 0, nullptr);
	cmdList->OMSetRenderTargets(1, &backBufferView, FALSE, nullptr);
}

void CommandQueue::RenderEnd()
{
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		swapChain->GetBackRTVBuffer().Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

	cmdList->ResourceBarrier(1, &barrier);

	cmdList->Close();

	ID3D12CommandList* cmdListArr[] = { cmdList.Get() };

	cmdQuene->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);
	swapChain->Present();

	WaitSync();

	swapChain->SwapIndex();
}

void CommandQueue::SubmitResourceCommandQueue()
{
	//리소스 명령 리스트를 닫음
	resCmdList->Close();

	//명령 리스트 배열 생성
	ID3D12CommandList* cmdListArr[] = { resCmdList.Get() };

	//명령 대기열에 있는 명령 리스트를 제출하여 실행
	cmdQuene->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);

	//동기화 대기
	WaitSync();

	//리소스 명령 할당자 재설정
	resCmdAlloc->Reset();

	//리소스 명령 리스트를 리소스 명령 할당자와 함께 재설정
	resCmdList->Reset(resCmdAlloc.Get(), nullptr);
}
