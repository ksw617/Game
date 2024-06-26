#include "pch.h"
#include "DepthStencilBuffer.h"
#include "Engine.h"

void DepthStencilBuffer::Init(int width, int heigh, DXGI_FORMAT _dsvFormat)
{
	//����/���ٽ� ���� ���˼���
	dsvFormat = _dsvFormat;

	//�� �Ӽ� ����(����Ʈ �� Ÿ��)
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	//���ҽ� ������ ����(�ؽ�ó 2D, ����/���ٽ� ����, �ʺ�, ����)
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Tex2D(dsvFormat, width, heigh);
	desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL; // ����/���ٽ� �÷��� ����

	//����ȭ�� Ŭ���� �� ����(���� 1.0, ���ٽ� 0)
	D3D12_CLEAR_VALUE optimizedClearValue = CD3DX12_CLEAR_VALUE(dsvFormat, 1.0f, 0);

	//����/���Ľ� ���� ���ҽ� ����
	Engine::Get().GetDevice()->GetDevice()->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&optimizedClearValue,
		IID_PPV_ARGS(&dsvBuffer));

	//��ũ���� �� ������ ����(����/���ٽ� ��)
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NumDescriptors = 1; // ��ũ���� �� 1��
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE; // �÷��� ����
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV; // ����/���ٽ� �� Ÿ��

	//��ũ���� �� ����
	Engine::Get().GetDevice()->GetDevice()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&dsvHeap));

	//���� / ���ٽ� ���� CPU ��ũ���� �ڵ� ��������
	dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();

	//���� / ���ٽ� �� ����
	Engine::Get().GetDevice()->GetDevice()->CreateDepthStencilView(dsvBuffer.Get(), nullptr, dsvHandle);
}
