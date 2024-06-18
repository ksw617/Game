#include "pch.h"
#include "ConstantBuffer.h"
#include "Engine.h"

ConstantBuffer::ConstantBuffer()
{
}

ConstantBuffer::~ConstantBuffer()
{
	if (cbvBuffer)
	{
		if (cbvBuffer != nullptr)
		{
			cbvBuffer->Unmap(0, nullptr); 
			
			cbvBuffer = nullptr;
		}
		
	}
}

//상수 버퍼 초기화
void ConstantBuffer::Init(int size, int count)
{
	//상수 버퍼는 256 바이트 배수로 만듬
	//0, 256, 512, 768.. 
	elementSize = (size + 255) & ~255; // 256 바이트 배수로 정렬
	elementCount = count;

	//버퍼 생성
	CreateBuffer();
}


void ConstantBuffer::CreateBuffer()
{
	//버퍼 크기 계산
	int bufferSize = elementSize * elementCount;
	//힙 속성 설정(업로드 힙)
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	//리소스 설명자 설정(버퍼)
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	//커밋된 리소스 생성
	Engine::Get().GetDevice()->GetDevice()->CreateCommittedResource
	(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&cbvBuffer)
	);

	//리소스 맵핑
	cbvBuffer->Map(0, nullptr, reinterpret_cast<void**>(&mappedBuffer));

	
}

//상수 버퍼 데이터 클리어
void ConstantBuffer::Clear()
{
	//현재 인덱스를 0으로 클리어
	currentIndex = 0;
}

void ConstantBuffer::PushData(int rootParamIndex, void* buffer, int size)
{
	// 인덱스 범위 확인
	assert(currentIndex < elementCount);
		   
	//데이터 복사
	memcpy(&mappedBuffer[currentIndex * elementSize], buffer, size);

	//GPU 가상 주소 가져오고
	D3D12_GPU_VIRTUAL_ADDRESS address = GetGpuVirtualAddress(currentIndex);

	//루트 시그니처에 상수 버퍼 뷰 설정
	Engine::Get().GetCmdQueue()->GetCmdList()->SetGraphicsRootConstantBufferView(rootParamIndex, address);
	currentIndex++;

}

D3D12_GPU_VIRTUAL_ADDRESS ConstantBuffer::GetGpuVirtualAddress(int index)
{
	//virtual 주소 가져오고
	D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = cbvBuffer->GetGPUVirtualAddress();
	objCBAddress += index * elementSize; // 인덱스를 기준으로 오프셋 추가

	return objCBAddress;;
}
