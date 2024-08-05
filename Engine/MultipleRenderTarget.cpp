#include "pch.h"
#include "MultipleRenderTarget.h"
#include "Engine.h"
#include "Device.h"

void MultipleRenderTarget::Create(RENDER_TARGET_TYPE type, vector<RenderTarget>& rtVec, shared_ptr<Texture> dsTexture)
{
	renderTargetType = type; // 렌더 타겟 타입 설정
	renderTargets = rtVec; // 렌더 타겟 벡터 설정
	rtCount = static_cast<UINT32>(renderTargets.size()); // 렌더 타겟 개수 설정
	dsTexture = dsTexture; // 깊이 스텐실 텍스처 설정

	// RTV 힙 설명자 초기화
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // RTV 타입 설정
	heapDesc.NumDescriptors = rtCount; // 디스크립터 개수 설정
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE; // 플래그 설정
	heapDesc.NodeMask = 0; // 노드 마스크 설정

	// RTV 힙 생성
	Engine::Get().GetDevice()->GetDevice()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeap));

	// RTV 힙 크기와 시작 위치 설정
	rtvHeapSize = Engine::Get().GetDevice()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	rtvHeapBegin = rtvHeap->GetCPUDescriptorHandleForHeapStart();
	dsvHeapBegin = dsTexture->GetDSV()->GetCPUDescriptorHandleForHeapStart();

	// 각 렌더 타겟에 대해 디스크립터 복사
	for (UINT32 i = 0; i < rtCount; i++)
	{
		UINT32 destSize = 1;
		D3D12_CPU_DESCRIPTOR_HANDLE destHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeapBegin, i * rtvHeapSize);

		UINT32 srcSize = 1;
		ComPtr<ID3D12DescriptorHeap> srcRtvHeapBegin = renderTargets[i].target->GetRTV();
		D3D12_CPU_DESCRIPTOR_HANDLE srcHandle = srcRtvHeapBegin->GetCPUDescriptorHandleForHeapStart();

		// 디스크립터 복사
		Engine::Get().GetDevice()->GetDevice()->CopyDescriptors(1, &destHandle, &destSize, 1, &srcHandle, &srcSize, D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}
}

// 렌더 타겟 설정 함수 (카운트와 오프셋을 사용하여 설정)
void MultipleRenderTarget::OMSetRenderTargets(UINT32 count, UINT32 offset)
{
	// 오프셋을 고려하여 RTV 핸들 계산
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeapBegin, offset * rtvHeapSize);

	// 명령 리스트에 렌더 타겟과 깊이 스텐실 뷰 설정
	Engine::Get().GetCmdQueue()->GetCmdList()->OMSetRenderTargets(count, &rtvHandle, FALSE /* 단일 RTV */, &dsvHeapBegin);
}

// 렌더 타겟 설정 함수 (다중 렌더 타겟 설정)
void MultipleRenderTarget::OMSetRenderTargets()
{
	// 명령 리스트에 모든 렌더 타겟과 깊이 스텐실 뷰 설정
	Engine::Get().GetCmdQueue()->GetCmdList()->OMSetRenderTargets(rtCount, &rtvHeapBegin, TRUE /* 다중 RTV */, &dsvHeapBegin);
}

// 특정 인덱스의 렌더 타겟 뷰를 클리어하는 함수
void MultipleRenderTarget::ClearRenderTargetView(UINT32 index)
{
	// 특정 인덱스의 RTV 핸들 계산
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeapBegin, index * rtvHeapSize);

	// 명령 리스트에 특정 RTV 클리어 명령 추가
	Engine::Get().GetCmdQueue()->GetCmdList()->ClearRenderTargetView(rtvHandle, renderTargets[index].clearColor, 0, nullptr);

	// 명령 리스트에 깊이 스텐실 뷰 클리어 명령 추가
	Engine::Get().GetCmdQueue()->GetCmdList()->ClearDepthStencilView(dsvHeapBegin, D3D12_CLEAR_FLAG_DEPTH, 1.f, 0, 0, nullptr);
}

// 특정 인덱스의 렌더 타겟 뷰를 클리어하는 함수
void MultipleRenderTarget::ClearRenderTargetView()
{
	// 모든 RTV를 순회하면서 클리어
	for (UINT32 i = 0; i < rtCount; i++)
	{
		// 특정 인덱스의 RTV 핸들 계산
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeapBegin, i * rtvHeapSize);

		// 명령 리스트에 특정 RTV 클리어 명령 추가
		Engine::Get().GetCmdQueue()->GetCmdList()->ClearRenderTargetView(rtvHandle, renderTargets[i].clearColor, 0, nullptr);
	}

	// 명령 리스트에 깊이 스텐실 뷰 클리어 명령 추가
	Engine::Get().GetCmdQueue()->GetCmdList()->ClearDepthStencilView(dsvHeapBegin, D3D12_CLEAR_FLAG_DEPTH, 1.f, 0, 0, nullptr);
}
