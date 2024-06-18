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

//��� ���� �ʱ�ȭ
void ConstantBuffer::Init(int size, int count)
{
	//��� ���۴� 256 ����Ʈ ����� ����
	//0, 256, 512, 768.. 
	elementSize = (size + 255) & ~255; // 256 ����Ʈ ����� ����
	elementCount = count;

	//���� ����
	CreateBuffer();
}


void ConstantBuffer::CreateBuffer()
{
	//���� ũ�� ���
	int bufferSize = elementSize * elementCount;
	//�� �Ӽ� ����(���ε� ��)
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	//���ҽ� ������ ����(����)
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	//Ŀ�Ե� ���ҽ� ����
	Engine::Get().GetDevice()->GetDevice()->CreateCommittedResource
	(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&cbvBuffer)
	);

	//���ҽ� ����
	cbvBuffer->Map(0, nullptr, reinterpret_cast<void**>(&mappedBuffer));

	
}

//��� ���� ������ Ŭ����
void ConstantBuffer::Clear()
{
	//���� �ε����� 0���� Ŭ����
	currentIndex = 0;
}

void ConstantBuffer::PushData(int rootParamIndex, void* buffer, int size)
{
	// �ε��� ���� Ȯ��
	assert(currentIndex < elementCount);
		   
	//������ ����
	memcpy(&mappedBuffer[currentIndex * elementSize], buffer, size);

	//GPU ���� �ּ� ��������
	D3D12_GPU_VIRTUAL_ADDRESS address = GetGpuVirtualAddress(currentIndex);

	//��Ʈ �ñ״�ó�� ��� ���� �� ����
	Engine::Get().GetCmdQueue()->GetCmdList()->SetGraphicsRootConstantBufferView(rootParamIndex, address);
	currentIndex++;

}

D3D12_GPU_VIRTUAL_ADDRESS ConstantBuffer::GetGpuVirtualAddress(int index)
{
	//virtual �ּ� ��������
	D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = cbvBuffer->GetGPUVirtualAddress();
	objCBAddress += index * elementSize; // �ε����� �������� ������ �߰�

	return objCBAddress;;
}
