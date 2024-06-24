#pragma once
class ConstantBuffer
{
private:
	ComPtr<ID3D12Resource> cbvBuffer;
	BYTE* mappedBuffer = nullptr;

	UINT32 elementSize = 0;
	UINT32 elementCount = 0;
	UINT32 currentIndex = 0;

	ComPtr<ID3D12DescriptorHeap> cbvDescHeap;			//CBV 디스크립터 힙
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandleBegin = {};	//디스크립터 힙의 시작 CPU 핸들
	UINT32 handleIncrementSize = 0;						//디스크립터 핸들의 증가 크기

	CBV_REGISTER cbvRegister = {};						//상수 버퍼 뷰(CBV) 레지스터

public:
	ConstantBuffer();
	~ConstantBuffer();
private:
	void CreateBuffer();
	//View 만들기
	void CreateView();
public:
	//상수 버퍼 초기화
	void Init(CBV_REGISTER reg, UINT32 size, UINT32 count);

	void Clear();
	//상수 버퍼에 데이터 푸시
	void PushData(void* buffer, UINT32 size);

	D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress(UINT32 index);

	//CPU 디스크립터 핸들 가져오기
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(UINT32 index);
};

