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
	//Register ���
	cbvRegister = reg;

	elementSize = (size + 255) & ~255; 
	elementCount = count;

	CreateBuffer();
	//View ����
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

	D3D12_DESCRIPTOR_HEAP_DESC cbvDesc = {};
	cbvDesc.NumDescriptors = elementCount;
	cbvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE; 
	cbvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;


	Engine::Get().GetDevice()->GetDevice()->CreateDescriptorHeap(&cbvDesc, IID_PPV_ARGS(&cbvDescHeap));

	// ��ũ���� ���� ���� CPU �ڵ� ��������
	cpuHandleBegin = cbvDescHeap->GetCPUDescriptorHandleForHeapStart();


	handleIncrementSize = Engine::Get().GetDevice()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);


	for (UINT32 i = 0; i < elementCount; i++)
	{
		D3D12_CPU_DESCRIPTOR_HANDLE cbvHandle = GetCPUHandle(i);

		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = cbvBuffer->GetGPUVirtualAddress() + static_cast<UINT64>(elementSize) * i;
		cbvDesc.SizeInBytes = elementSize; 

		Engine::Get().GetDevice()->GetDevice()->CreateConstantBufferView(&cbvDesc, cbvHandle);

	}
}



//��� ���� ������ Ŭ����
void ConstantBuffer::Clear()
{
	//���� �ε����� 0���� Ŭ����
	currentIndex = 0;
}

void ConstantBuffer::PushData(void* buffer, UINT32 size)
{

	assert(currentIndex < elementCount);
	//�Էµ� ������ ũ�Ⱑ ��� ũ��� ��ġ�ϴ��� Ȯ�� (256����Ʈ ���)
	assert(elementSize == ((size + 255) & ~255));


	memcpy(&mappedBuffer[currentIndex * elementSize], buffer, size);


	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = GetCPUHandle(currentIndex);


	Engine::Get().GetTableDesc()->SetCBV(cpuHandle, cbvRegister);

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
	//���� CPU �ڵ鿡 �ε����� ���� �������� ���� ���� ��ȯ
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(cpuHandleBegin, index * handleIncrementSize);
}
