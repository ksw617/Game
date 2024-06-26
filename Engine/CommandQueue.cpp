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
	
	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&resCmdAlloc));
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
	//cmdList->OMSetRenderTargets(1, &backBufferView, FALSE, nullptr);


	//깊이/스텐실 뷰의 CPU 디스크립터 핸들을 가져옴
	D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView = Engine::Get().GetDepthStencilBuffer()->GetDSVCPUHandle();
	//출력 병합 단계에 랜더 타겟과 깊이/스탠실 뷰를 설정
	cmdList->OMSetRenderTargets(1, &backBufferView, FALSE, &depthStencilView);

	//깊이/스텐실 뷰를 초기화
	//깊이 값을 1.0으로 초기화 하고, 스텐실 값을 0으로 초기화
	cmdList->ClearDepthStencilView(depthStencilView, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
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
	resCmdList->Close();

	ID3D12CommandList* cmdListArr[] = { resCmdList.Get() };
	cmdQuene->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);

	WaitSync();

	resCmdAlloc->Reset();

	resCmdList->Reset(resCmdAlloc.Get(), nullptr);
}
