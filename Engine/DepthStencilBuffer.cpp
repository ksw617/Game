#include "pch.h"
#include "DepthStencilBuffer.h"

void DepthStencilBuffer::Init(int width, int heigh, DXGI_FORMAT _dsvFormat)
{
	//깊이/스텐실 뷰의 포맷설정
	dsvFormat = _dsvFormat;

	//힙 속성 설정(디폴트 힙 타입)
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	//리소스 설명자 설정(텍스처 2D, 깊이/스텐실 포맷, 너비, 높이)
	//D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Tex2D(dsvFormat, width, heigh);
	 
}
