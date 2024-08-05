#include "pch.h"
#include "MultipleRenderTarget.h"
#include "Engine.h"
#include "Device.h"

void MultipleRenderTarget::Create(RENDER_TARGET_TYPE type, vector<RenderTarget>& rtVec, shared_ptr<Texture> dsTexture)
{
	renderTargetType = type; // ���� Ÿ�� Ÿ�� ����
	renderTargets = rtVec; // ���� Ÿ�� ���� ����
	rtCount = static_cast<UINT32>(renderTargets.size()); // ���� Ÿ�� ���� ����
	dsTexture = dsTexture; // ���� ���ٽ� �ؽ�ó ����

	// RTV �� ������ �ʱ�ȭ
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // RTV Ÿ�� ����
	heapDesc.NumDescriptors = rtCount; // ��ũ���� ���� ����
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE; // �÷��� ����
	heapDesc.NodeMask = 0; // ��� ����ũ ����

	// RTV �� ����
	Engine::Get().GetDevice()->GetDevice()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeap));

	// RTV �� ũ��� ���� ��ġ ����
	rtvHeapSize = Engine::Get().GetDevice()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	rtvHeapBegin = rtvHeap->GetCPUDescriptorHandleForHeapStart();
	dsvHeapBegin = dsTexture->GetDSV()->GetCPUDescriptorHandleForHeapStart();

	// �� ���� Ÿ�ٿ� ���� ��ũ���� ����
	for (UINT32 i = 0; i < rtCount; i++)
	{
		UINT32 destSize = 1;
		D3D12_CPU_DESCRIPTOR_HANDLE destHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeapBegin, i * rtvHeapSize);

		UINT32 srcSize = 1;
		ComPtr<ID3D12DescriptorHeap> srcRtvHeapBegin = renderTargets[i].target->GetRTV();
		D3D12_CPU_DESCRIPTOR_HANDLE srcHandle = srcRtvHeapBegin->GetCPUDescriptorHandleForHeapStart();

		// ��ũ���� ����
		Engine::Get().GetDevice()->GetDevice()->CopyDescriptors(1, &destHandle, &destSize, 1, &srcHandle, &srcSize, D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}
}

// ���� Ÿ�� ���� �Լ� (ī��Ʈ�� �������� ����Ͽ� ����)
void MultipleRenderTarget::OMSetRenderTargets(UINT32 count, UINT32 offset)
{
	// �������� ����Ͽ� RTV �ڵ� ���
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeapBegin, offset * rtvHeapSize);

	// ��� ����Ʈ�� ���� Ÿ�ٰ� ���� ���ٽ� �� ����
	Engine::Get().GetCmdQueue()->GetCmdList()->OMSetRenderTargets(count, &rtvHandle, FALSE /* ���� RTV */, &dsvHeapBegin);
}

// ���� Ÿ�� ���� �Լ� (���� ���� Ÿ�� ����)
void MultipleRenderTarget::OMSetRenderTargets()
{
	// ��� ����Ʈ�� ��� ���� Ÿ�ٰ� ���� ���ٽ� �� ����
	Engine::Get().GetCmdQueue()->GetCmdList()->OMSetRenderTargets(rtCount, &rtvHeapBegin, TRUE /* ���� RTV */, &dsvHeapBegin);
}

// Ư�� �ε����� ���� Ÿ�� �並 Ŭ�����ϴ� �Լ�
void MultipleRenderTarget::ClearRenderTargetView(UINT32 index)
{
	// Ư�� �ε����� RTV �ڵ� ���
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeapBegin, index * rtvHeapSize);

	// ��� ����Ʈ�� Ư�� RTV Ŭ���� ��� �߰�
	Engine::Get().GetCmdQueue()->GetCmdList()->ClearRenderTargetView(rtvHandle, renderTargets[index].clearColor, 0, nullptr);

	// ��� ����Ʈ�� ���� ���ٽ� �� Ŭ���� ��� �߰�
	Engine::Get().GetCmdQueue()->GetCmdList()->ClearDepthStencilView(dsvHeapBegin, D3D12_CLEAR_FLAG_DEPTH, 1.f, 0, 0, nullptr);
}

// Ư�� �ε����� ���� Ÿ�� �並 Ŭ�����ϴ� �Լ�
void MultipleRenderTarget::ClearRenderTargetView()
{
	// ��� RTV�� ��ȸ�ϸ鼭 Ŭ����
	for (UINT32 i = 0; i < rtCount; i++)
	{
		// Ư�� �ε����� RTV �ڵ� ���
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeapBegin, i * rtvHeapSize);

		// ��� ����Ʈ�� Ư�� RTV Ŭ���� ��� �߰�
		Engine::Get().GetCmdQueue()->GetCmdList()->ClearRenderTargetView(rtvHandle, renderTargets[i].clearColor, 0, nullptr);
	}

	// ��� ����Ʈ�� ���� ���ٽ� �� Ŭ���� ��� �߰�
	Engine::Get().GetCmdQueue()->GetCmdList()->ClearDepthStencilView(dsvHeapBegin, D3D12_CLEAR_FLAG_DEPTH, 1.f, 0, 0, nullptr);
}
