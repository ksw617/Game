#pragma once
#include "Object.h"

//Object 상속
class Texture : public Object
{
private:
	ScratchImage image; 
	ComPtr<ID3D12Resource> texture2D;

	ComPtr<ID3D12DescriptorHeap> srvHeap;	
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle;	
public:
	//타입 초기화
	Texture() : Object(OBJECT_TYPE::TEXTURE) {}
	virtual ~Texture() {}
public:
	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle() { return srvHandle; }
public:
	void Init(const wstring& path);
private:
	void CreateTexture(const wstring& path);
	void CreateView();

};

