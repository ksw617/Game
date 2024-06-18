#pragma once
class ConstantBuffer
{
private:
	ComPtr<ID3D12Resource> cbvBuffer;
	BYTE* mappedBuffer = nullptr;
	int elementSize = 0;
	int elementCount = 0;
	int currentIndex = 0;

public:
	ConstantBuffer();
	~ConstantBuffer();
private:
	//상수 버퍼 생성
	void CreateBuffer();
public:
	//상수 버퍼 초기화
	void Init(int size, int count);
	//상수 버퍼 데이터 초기화
	void Clear();
	//상수 버퍼에 데이터 푸시
	void PushData(int rootParamIndex, void* buffer, int size);

	//GPU 가상 주소 가져오기
	D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress(int index);

};

