#include "pch.h"
#include "DepthStencilBuffer.h"

void DepthStencilBuffer::Init(int width, int heigh, DXGI_FORMAT _dsvFormat)
{
	//����/���ٽ� ���� ���˼���
	dsvFormat = _dsvFormat;

	//�� �Ӽ� ����(����Ʈ �� Ÿ��)
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	//���ҽ� ������ ����(�ؽ�ó 2D, ����/���ٽ� ����, �ʺ�, ����)
	//D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Tex2D(dsvFormat, width, heigh);
	 
}
