#pragma once
							 

enum class CONSTANT_BUFFER_TYPE
{
	GLOBAL,		//傈开 单捞磐 包府
	TRANSFORM,	
	MATERIAL,	
	END,		

};

enum 
{
	CONSTANT_BUFFER_COUNT = static_cast<UINT8>(CONSTANT_BUFFER_TYPE::END)

};


class ConstantBuffer
{
private:
	ComPtr<ID3D12Resource> cbvBuffer;
	BYTE* mappedBuffer = nullptr;

	UINT32 elementSize = 0;
	UINT32 elementCount = 0;
	UINT32 currentIndex = 0;

	ComPtr<ID3D12DescriptorHeap> cbvDescHeap;			
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandleBegin = {};	
	UINT32 handleIncrementSize = 0;						

	CBV_REGISTER cbvRegister = {};						

public:
	ConstantBuffer();
	~ConstantBuffer();
private:
	void CreateBuffer();
	void CreateView();
public:
	D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress(UINT32 index);
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(UINT32 index);
public:
	void Init(CBV_REGISTER reg, UINT32 size, UINT32 count);
	void Clear();
	void PushData(void* buffer, UINT32 size);

	//傈开 Data 持扁
	void PushGlobalData(void* buffer, UINT32 size);


};

