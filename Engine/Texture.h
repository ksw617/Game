#pragma once
#include "Object.h"

//Object 상속
class Texture : public Object
{
private:
	ScratchImage image; 
	ComPtr<ID3D12Resource> texture2D;

	ComPtr<ID3D12DescriptorHeap> srvHeap;	
	//rtv & dsv용 만들기
	ComPtr<ID3D12DescriptorHeap> rtvHeap; // 렌더 타겟 뷰 힙
	ComPtr<ID3D12DescriptorHeap> dsvHeap; // 깊이 스텐실 뷰 힙

	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle;	
public:
	//타입 초기화
	Texture() : Object(OBJECT_TYPE::TEXTURE) {}
	virtual ~Texture() {}
public:
	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle() { return srvHandle; }
public:
	//Get함수
	// 2D 텍스처 리소스를 반환하는 함수
	ComPtr<ID3D12Resource> GetTexture2D() { return texture2D; }
	// 쉐이더 리소스 뷰 힙을 반환하는 함수
	ComPtr<ID3D12DescriptorHeap> GetSRV() { return srvHeap; }
	// 렌더 타겟 뷰 힙을 반환하는 함수
	ComPtr<ID3D12DescriptorHeap> GetRTV() { return rtvHeap; }
	// 깊이 스텐실 뷰 힙을 반환하는 함수
	ComPtr<ID3D12DescriptorHeap> GetDSV() { return dsvHeap; }

public:
	virtual void Load(const wstring& path) override;
public:
	// 텍스처를 생성하는 함수
	void Create(DXGI_FORMAT format, UINT32 width, UINT32 height,
		const D3D12_HEAP_PROPERTIES& heapProperty, D3D12_HEAP_FLAGS heapFlags,
		D3D12_RESOURCE_FLAGS resFlags, Vector4 clearColor = Vector4());

	// 리소스로부터 텍스처를 생성하는 함수
	void CreateFromResource(ComPtr<ID3D12Resource> tex2D);

private:
	void CreateTexture(const wstring& path);
	void CreateView();

};

