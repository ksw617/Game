#include "pch.h"
#include "DepthStencilBuffer.h"
#include "Engine.h"

void DepthStencilBuffer::Init(int width, int heigh, DXGI_FORMAT _dsvFormat)
{
	//깊이/스텐실 뷰의 포맷설정
	dsvFormat = _dsvFormat;

	//힙 속성 설정(디폴트 힙 타입)
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	//리소스 설명자 설정(텍스처 2D, 깊이/스텐실 포맷, 너비, 높이)
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Tex2D(dsvFormat, width, heigh);
	desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL; // 깊이/스텐실 플래그 설정

	//최적화된 클리어 값 설정(깊이 1.0, 스텐실 0)
	D3D12_CLEAR_VALUE optimizedClearValue = CD3DX12_CLEAR_VALUE(dsvFormat, 1.0f, 0);

	//깊이/스탠실 버퍼 리소스 생성
	Engine::Get().GetDevice()->GetDevice()->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&optimizedClearValue,
		IID_PPV_ARGS(&dsvBuffer));

	//디스크립터 힙 설명자 설정(깊이/스텐실 뷰)
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NumDescriptors = 1; // 디스크립터 수 1개
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE; // 플래그 없음
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV; // 깊이/스텐실 뷰 타입

	//디스크립터 힙 생성
	Engine::Get().GetDevice()->GetDevice()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&dsvHeap));

	//깊이 / 스텐실 뷰의 CPU 디스크립터 핸들 가져오기
	dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();

	//깊이 / 스텐실 뷰 생성
	Engine::Get().GetDevice()->GetDevice()->CreateDepthStencilView(dsvBuffer.Get(), nullptr, dsvHandle);
}
