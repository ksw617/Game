#pragma once
class DepthStencilBuffer
{
private:
	//깊이/스텐실 버퍼 리소스
	ComPtr<ID3D12Resource> dsvBuffer;
	//깊이/스텐실 뷰 디스크립터 힙
	ComPtr<ID3D12DescriptorHeap> dsvHeap;
	//깊이/스텐실 뷰의 CPU 디스크립터 핸들
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = {};
	//깊이/스텐실 뷰의 포멧
	DXGI_FORMAT dsvFormat = {};
public:
	//깊이/스텐실 뷰의 CPU 디스크립터 핸들은 반환하는 함수
	D3D12_CPU_DESCRIPTOR_HANDLE GetDSVCPUHandle() { return dsvHandle; }
	//깊이/스텐실 뷰의 포멧을 반환하는 함수
	DXGI_FORMAT GetDSVFormat() { return dsvFormat; }
public:
	//깊이/스텐실 버퍼의 초기화
	void Init(int width, int heigh, DXGI_FORMAT _dsvFormat = DXGI_FORMAT_D32_FLOAT);
};
