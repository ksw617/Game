#pragma once
class Texture
{
private:
	ScratchImage image; // 이미지 데이터를 저장하는 ScratchImage 객체
	ComPtr<ID3D12Resource> texture2D; // 텍스처 리소스

	ComPtr<ID3D12DescriptorHeap> srvHeap;	// 쉐이더 리소스 뷰(SRV) 디스크립터 힙
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle;	// SRV의 CPU 디스크립터 핸들

public:
	//SRV의 CPU 디스크립터 핸들을 반환 하는 함수
	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle() { return srvHandle; }
public:
	//텍스처를 초기화하는 함수(파일 경로를 인자로 받음)
	void Init(const wstring& path);
private:
	//파일 경로를 받아 텍스처를 생성하는 함수
	void CreateTexture(const wstring& path);
	//텍스처에 대한 뷰(SRV)를 생성하는 함수
	void CreateView();

};

