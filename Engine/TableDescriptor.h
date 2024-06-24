#pragma once
class TableDescriptor
{
private:
	ComPtr<ID3D12DescriptorHeap> descHeap;	
	UINT64 handleSize = 0;					
	UINT64 groupSize = 0;					
	UINT64 groupCount = 0;					

	UINT32 currentGroupIndex = 0;			
public:

	ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap() { return descHeap; }
public:
	void Init(UINT32 count);
	void Clear();
	void SetCBV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, CBV_REGISTER reg);
	//디스크립터 테이블을 커밋
	void CommitTable();
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(CBV_REGISTER reg);


private:
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(UINT32 reg);

};

