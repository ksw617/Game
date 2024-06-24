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

void ConstantBuffer::Init(CBV_REGISTER reg, UINT32 size, UINT32 count)
{
	//Register 등록
	cbvRegister = reg;

	elementSize = (size + 255) & ~255; 
	elementCount = count;

	CreateBuffer();
	//View 생성
	CreateView();
}


void ConstantBuffer::CreateBuffer()
{
	int bufferSize = elementSize * elementCount;
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	Engine::Get().GetDevice()->GetDevice()->CreateCommittedResource
	(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&cbvBuffer)
	);

	cbvBuffer->Map(0, nullptr, reinterpret_cast<void**>(&mappedBuffer));

	
}

void ConstantBuffer::CreateView()
{
	//디스크립터 힙 디스크립트 설정
	D3D12_DESCRIPTOR_HEAP_DESC cbvDesc = {};
	cbvDesc.NumDescriptors = elementCount;	//디스크립터 수는 요소의 수와 동일
	cbvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE; // 플래그 없음
	cbvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV; // 힙 타입은 CBV/SRV/UAV

	//디스크립터 힙 생성
	Engine::Get().GetDevice()->GetDevice()->CreateDescriptorHeap(&cbvDesc, IID_PPV_ARGS(&cbvDescHeap));

	//디스크립터 핸들 증가 크기 가져오기
	handleIncrementSize = Engine::Get().GetDevice()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//각 요소에 대해 상수 버퍼 뷰(CBV) 생성
	for (UINT32 i = 0; i < elementCount; i++)
	{
		//현재 요소의 CPU 디스크립터 핸들 가져오기
		D3D12_CPU_DESCRIPTOR_HANDLE cbvHandle = GetCPUHandle(i);

		//상수 버퍼 뷰 디스크립트 설정
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		//GPU 가상 주소 설정
		cbvDesc.BufferLocation = cbvBuffer->GetGPUVirtualAddress() + static_cast<UINT64>(elementSize) * i;
		cbvDesc.SizeInBytes = elementSize; // 상수 버퍼 크기는 elememtSize: 256바이트 배수로 설정

		//상수 버퍼 뷰(CBV) 생성
		Engine::Get().GetDevice()->GetDevice()->CreateConstantBufferView(&cbvDesc, cbvHandle);

	}
}



//상수 버퍼 데이터 클리어
void ConstantBuffer::Clear()
{
	//현재 인덱스를 0으로 클리어
	currentIndex = 0;
}

void ConstantBuffer::PushData(void* buffer, UINT32 size)
{
	// 인덱스 범위 확인
	assert(currentIndex < elementCount);
	//입력된 데이터 크기가 요소 크기와 일치하는지 확인 (256바이트 배수)
	assert(elementSize == ((size + 255) & ~355));

	//데이터 복사
	memcpy(&mappedBuffer[currentIndex * elementSize], buffer, size);

	//현재 인덱스에 대한 CPU 디스크립터 핸들을 가져옴
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = GetCPUHandle(currentIndex);

	//디스크립터 힙에 상수 버퍼 뷰(CBV) 설정
	Engine::Get().GetTableDesc()->SetCBV(cpuHandle, cbvRegister);

	//현재 인덱스를 증가
	currentIndex++;
}

D3D12_GPU_VIRTUAL_ADDRESS ConstantBuffer::GetGpuVirtualAddress(UINT32 index)
{

	D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = cbvBuffer->GetGPUVirtualAddress();
	objCBAddress += index * elementSize;

	return objCBAddress;;
}

D3D12_CPU_DESCRIPTOR_HANDLE ConstantBuffer::GetCPUHandle(UINT32 index)
{
	//시작 CPU 핸들에 인덱스에 따른 오프셋을 더한 값을 반환
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(cpuHandleBegin, index * handleIncrementSize);
}
