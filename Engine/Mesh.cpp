#include "pch.h"
#include "Mesh.h"
#include "Engine.h"

void Mesh::Init(vector<Vertex>& vec)
{
	//������ ������ ����
	vertexCount = static_cast<int>(vec.size());
	//���� ũ�� ���
	int bufferSize = vertexCount * sizeof(Vertex);

	//�� �Ӽ� ����(���ε� ��)
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	//���ҽ� ������ ���� (����) 
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	//Ŀ�� ���ҽ� ����
	Engine::Get().GetDevice()->GetDevice()->CreateCommittedResource(
	  &heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertexBuffer));

	//���� �����͸� ���ۿ� ����
	void* vertexDataBuffer = nullptr;
	CD3DX12_RANGE readRange(0,0);// �б� ���� ����
	vertexBuffer->Map(0, &readRange, &vertexDataBuffer);
	memcpy(vertexDataBuffer, &vec[0], bufferSize);
	vertexBuffer->Unmap(0, nullptr);

	//���� ���� �� ����
	vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
	vertexBufferView.StrideInBytes = sizeof(Vertex);
	vertexBufferView.SizeInBytes = bufferSize;
	
}

//�޽��� ������
void Mesh::Render()
{
	//�⺻ ������Ƽ�� ���������� �ﰢ�� ����Ʈ�� ����
	Engine::Get().GetCmdQueue()->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//���� ���۸� ���� 0�� ���� (���� : 0~15)
	Engine::Get().GetCmdQueue()->GetCmdList()->IASetVertexBuffers(0, 1, &vertexBufferView);
	//�ν��Ͻ��� �׸���
	Engine::Get().GetCmdQueue()->GetCmdList()->DrawInstanced(vertexCount, 1, 0, 0);

}
