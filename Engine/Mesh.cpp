#include "pch.h"
#include "Mesh.h"
#include "Engine.h"

void Mesh::Init(const vector<Vertex>& vertexBuffer, const vector<UINT32>& indexBuffer)
{
	CreateVertexBuffer(vertexBuffer);
	CreateIndexBuffer(indexBuffer);
}

void Mesh::CreateVertexBuffer(const vector<Vertex>& buffer)
{
	vertexCount = static_cast<int>(buffer.size());
	int bufferSize = vertexCount * sizeof(Vertex);

	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	Engine::Get().GetDevice()->GetDevice()->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertexBuffer));

	void* vertexDataBuffer = nullptr;
	CD3DX12_RANGE readRange(0, 0);
	vertexBuffer->Map(0, &readRange, &vertexDataBuffer);
	memcpy(vertexDataBuffer, &buffer[0], bufferSize);
	vertexBuffer->Unmap(0, nullptr);

	vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
	vertexBufferView.StrideInBytes = sizeof(Vertex);
	vertexBufferView.SizeInBytes = bufferSize;
}




void Mesh::CreateIndexBuffer(const vector<UINT32>& buffer)
{
	indexCount = static_cast<int>(buffer.size());
	int bufferSize = indexCount * sizeof(UINT32);

	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	Engine::Get().GetDevice()->GetDevice()->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuffer));

	void* indexDataBuffer = nullptr;
	CD3DX12_RANGE readRange(0, 0); 
	indexBuffer->Map(0, &readRange, &indexDataBuffer); 
	memcpy(indexDataBuffer, &buffer[0], bufferSize);  
	indexBuffer->Unmap(0, nullptr);

	indexBufferView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
	indexBufferView.Format = DXGI_FORMAT_R32_UINT; 
	indexBufferView.SizeInBytes = bufferSize;
}

void Mesh::Render()
{
	Engine::Get().GetCmdQueue()->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Engine::Get().GetCmdQueue()->GetCmdList()->IASetVertexBuffers(0, 1, &vertexBufferView);


	Engine::Get().GetCmdQueue()->GetCmdList()->IASetIndexBuffer(&indexBufferView);

	Engine::Get().GetConstBuffer()->PushData(&transform, sizeof(transform));

	//SRV Register에 텍스처 등록
	Engine::Get().GetTableDesc()->SetSRV(texture->GetCpuHandle(), SRV_REGISTER::t0);

	Engine::Get().GetTableDesc()->CommitTable();


	Engine::Get().GetCmdQueue()->GetCmdList()->DrawIndexedInstanced(indexCount, 1, 0, 0, 0);

}
