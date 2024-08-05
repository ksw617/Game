#pragma once
#include "Object.h"

//Object ���
class Texture : public Object
{
private:
	ScratchImage image; 
	ComPtr<ID3D12Resource> texture2D;

	ComPtr<ID3D12DescriptorHeap> srvHeap;	
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle;	
public:
	//Ÿ�� �ʱ�ȭ
	Texture() : Object(OBJECT_TYPE::TEXTURE) {}
	virtual ~Texture() {}
public:
	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle() { return srvHandle; }
public:
	virtual void Load(const wstring& path) override;
private:
	void CreateTexture(const wstring& path);
	void CreateView();

};

