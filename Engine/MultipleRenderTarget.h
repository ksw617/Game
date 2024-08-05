#pragma once
#include "Texture.h"

// ���� Ÿ���� Ÿ���� �����ϴ� ������
enum class RENDER_TARGET_TYPE : UINT8
{
	SWAP_CHAIN, // BACK_BUFFER, FRONT_BUFFER
	BUFFER, // POSITION, NORMAL, COLOR
	END,
};

// ���� Ÿ�� ���� ��� ������ ���� Ÿ�� ������ �����ϴ� ���
enum
{
	RENDER_TARGET_BUFFER_MEMBER_COUNT = 3,
	RENDER_TARGET_COUNT = static_cast<UINT8>(RENDER_TARGET_TYPE::END)
};

// ���� Ÿ�� ����ü ����
struct RenderTarget
{
	shared_ptr<Texture> target; // ���� Ÿ�� �ؽ�ó
	float clearColor[4]; // Ŭ���� ����
};

// ��Ƽ�� ���� Ÿ�� Ŭ���� ����
class MultipleRenderTarget
{
private:
	RENDER_TARGET_TYPE	renderTargetType;	// ���� Ÿ�� Ÿ��
	vector<RenderTarget> renderTargets;		// ���� Ÿ�� ����
	UINT32 rtCount;							// ���� Ÿ�� ����
	shared_ptr<Texture> dsTexture;			// ���� ���ٽ� �ؽ�ó
	ComPtr<ID3D12DescriptorHeap> rtvHeap;	// ���� Ÿ�� �� ��
private:
	UINT32					rtvHeapSize; // RTV �� ũ��
	D3D12_CPU_DESCRIPTOR_HANDLE		rtvHeapBegin; // RTV �� ���� ��ġ
	D3D12_CPU_DESCRIPTOR_HANDLE		dsvHeapBegin; // DSV �� ���� ��ġ
public:
	// ���� Ÿ�� �ؽ�ó�� ��ȯ�ϴ� �Լ�
	shared_ptr<Texture> GetRTTexture(UINT32 index) { return renderTargets[index].target; }
	// ���� ���ٽ� �ؽ�ó�� ��ȯ�ϴ� �Լ�
	shared_ptr<Texture> GetDSTexture() { return dsTexture; }
public:
	// ���� Ÿ���� �����ϴ� �Լ�
	void Create(RENDER_TARGET_TYPE type, vector<RenderTarget>& rtVec, shared_ptr<Texture> dsTexture);

	// ���� Ÿ���� �����ϴ� �Լ� (ī��Ʈ�� �������� �̿�)
	void OMSetRenderTargets(UINT32 count, UINT32 offset);
	// ���� Ÿ���� �����ϴ� �Լ�
	void OMSetRenderTargets();

	// Ư�� �ε����� ���� Ÿ�� �並 Ŭ�����ϴ� �Լ�
	void ClearRenderTargetView(UINT32 index);
	// ��� ���� Ÿ�� �並 Ŭ�����ϴ� �Լ�
	void ClearRenderTargetView();
};

