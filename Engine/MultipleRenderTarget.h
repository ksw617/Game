#pragma once
#include "Texture.h"

// 렌더 타겟의 타입을 정의하는 열거형
enum class RENDER_TARGET_TYPE : UINT8
{
	SWAP_CHAIN, // BACK_BUFFER, FRONT_BUFFER
	BUFFER, // POSITION, NORMAL, COLOR
	END,
};

// 렌더 타겟 버퍼 멤버 개수와 렌더 타겟 개수를 정의하는 상수
enum
{
	RENDER_TARGET_BUFFER_MEMBER_COUNT = 3,
	RENDER_TARGET_COUNT = static_cast<UINT8>(RENDER_TARGET_TYPE::END)
};

// 렌더 타겟 구조체 정의
struct RenderTarget
{
	shared_ptr<Texture> target; // 렌더 타겟 텍스처
	float clearColor[4]; // 클리어 색상
};

// 멀티플 렌더 타겟 클래스 정의
class MultipleRenderTarget
{
private:
	RENDER_TARGET_TYPE	renderTargetType;	// 렌더 타겟 타입
	vector<RenderTarget> renderTargets;		// 렌더 타겟 벡터
	UINT32 rtCount;							// 렌더 타겟 개수
	shared_ptr<Texture> dsTexture;			// 깊이 스텐실 텍스처
	ComPtr<ID3D12DescriptorHeap> rtvHeap;	// 렌더 타겟 뷰 힙
private:
	UINT32					rtvHeapSize; // RTV 힙 크기
	D3D12_CPU_DESCRIPTOR_HANDLE		rtvHeapBegin; // RTV 힙 시작 위치
	D3D12_CPU_DESCRIPTOR_HANDLE		dsvHeapBegin; // DSV 힙 시작 위치
public:
	// 렌더 타겟 텍스처를 반환하는 함수
	shared_ptr<Texture> GetRTTexture(UINT32 index) { return renderTargets[index].target; }
	// 깊이 스텐실 텍스처를 반환하는 함수
	shared_ptr<Texture> GetDSTexture() { return dsTexture; }
public:
	// 렌더 타겟을 생성하는 함수
	void Create(RENDER_TARGET_TYPE type, vector<RenderTarget>& rtVec, shared_ptr<Texture> dsTexture);

	// 렌더 타겟을 설정하는 함수 (카운트와 오프셋을 이용)
	void OMSetRenderTargets(UINT32 count, UINT32 offset);
	// 렌더 타겟을 설정하는 함수
	void OMSetRenderTargets();

	// 특정 인덱스의 렌더 타겟 뷰를 클리어하는 함수
	void ClearRenderTargetView(UINT32 index);
	// 모든 렌더 타겟 뷰를 클리어하는 함수
	void ClearRenderTargetView();
};

