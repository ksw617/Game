#pragma once
#include "Object.h"

//Object ���
class Texture : public Object
{
private:
	ScratchImage image; 
	ComPtr<ID3D12Resource> texture2D;

	ComPtr<ID3D12DescriptorHeap> srvHeap;	
	//rtv & dsv�� �����
	ComPtr<ID3D12DescriptorHeap> rtvHeap; // ���� Ÿ�� �� ��
	ComPtr<ID3D12DescriptorHeap> dsvHeap; // ���� ���ٽ� �� ��

	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle;	
public:
	//Ÿ�� �ʱ�ȭ
	Texture() : Object(OBJECT_TYPE::TEXTURE) {}
	virtual ~Texture() {}
public:
	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle() { return srvHandle; }
public:
	//Get�Լ�
	// 2D �ؽ�ó ���ҽ��� ��ȯ�ϴ� �Լ�
	ComPtr<ID3D12Resource> GetTexture2D() { return texture2D; }
	// ���̴� ���ҽ� �� ���� ��ȯ�ϴ� �Լ�
	ComPtr<ID3D12DescriptorHeap> GetSRV() { return srvHeap; }
	// ���� Ÿ�� �� ���� ��ȯ�ϴ� �Լ�
	ComPtr<ID3D12DescriptorHeap> GetRTV() { return rtvHeap; }
	// ���� ���ٽ� �� ���� ��ȯ�ϴ� �Լ�
	ComPtr<ID3D12DescriptorHeap> GetDSV() { return dsvHeap; }

public:
	virtual void Load(const wstring& path) override;
public:
	// �ؽ�ó�� �����ϴ� �Լ�
	void Create(DXGI_FORMAT format, UINT32 width, UINT32 height,
		const D3D12_HEAP_PROPERTIES& heapProperty, D3D12_HEAP_FLAGS heapFlags,
		D3D12_RESOURCE_FLAGS resFlags, Vector4 clearColor = Vector4());

	// ���ҽ��κ��� �ؽ�ó�� �����ϴ� �Լ�
	void CreateFromResource(ComPtr<ID3D12Resource> tex2D);

private:
	void CreateTexture(const wstring& path);
	void CreateView();

};

