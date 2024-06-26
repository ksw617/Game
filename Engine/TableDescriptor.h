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
	//SRV용 Set 함수
	void SetSRV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, SRV_REGISTER reg);
	
	void CommitTable();
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(CBV_REGISTER reg);
	//SRV용 Get Handle
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(SRV_REGISTER reg);


private:
	//8비트로 변경
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(UINT8 reg);

};

