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

	//swapChain에서 BufferIndex값 불러오기
	UINT8 backBufferIndex = swapChain->GetBackBufferIndex();

	//수정
	//D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
	//	swapChain->GetBackRTVBuffer().Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	
	//변경
	//swapChain->GetBackRTVBuffer().Get() 이것을
	//Engine::Get().GetMRT(RENDER_TARGET_TYPE::SWAP_CHAIN)->GetRTTexture(backBufferIndex)->GetTexture2D().Get()	 이걸로 변경
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		Engine::Get().GetMRT(RENDER_TARGET_TYPE::SWAP_CHAIN)->GetRTTexture(backBufferIndex)->GetTexture2D().Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	
	cmdList->SetGraphicsRootSignature(Engine::Get().GetRootSignature()->GetSignature().Get());
	
	Engine::Get().GetConstantBuffer(CONSTANT_BUFFER_TYPE::TRANSFORM)->Clear();
	Engine::Get().GetConstantBuffer(CONSTANT_BUFFER_TYPE::MATERIAL)->Clear();

	Engine::Get().GetTableDesc()->Clear();

	ID3D12DescriptorHeap* descHeap = Engine::Get().GetTableDesc()->GetDescriptorHeap().Get();
	cmdList->SetDescriptorHeaps(1, &descHeap);

	cmdList->ResourceBarrier(1, &barrier);
	cmdList->RSSetViewports(1, vp);
	cmdList->RSSetScissorRects(1, rect);


	//사용안함
	//D3D12_CPU_DESCRIPTOR_HANDLE backBufferView = swapChain->GetBackRTV();
	//
	//cmdList->ClearRenderTargetView(backBufferView, Colors::Black, 0, nullptr);
	//
	//D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView = Engine::Get().GetDepthStencilBuffer()->GetDSVCPUHandle();
	//
	//cmdList->OMSetRenderTargets(1, &backBufferView, FALSE, &depthStencilView);
	//cmdList->ClearDepthStencilView(depthStencilView, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void CommandQueue::RenderEnd()
{

	//swapChain에서 BufferIndex값 불러오기
	UINT8 backBufferIndex = swapChain->GetBackBufferIndex();

	//D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
	//	swapChain->GetBackRTVBuffer().Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

	//변경
	//swapChain->GetBackRTVBuffer().Get() 이것을
	//Engine::Get().GetMRT(RENDER_TARGET_TYPE::SWAP_CHAIN)->GetRTTexture(backBufferIndex)->GetTexture2D().Get()	 이걸로 변경
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		Engine::Get().GetMRT(RENDER_TARGET_TYPE::SWAP_CHAIN)->GetRTTexture(backBufferIndex)->GetTexture2D().Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);


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
