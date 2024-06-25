#include "pch.h"
#include "Mesh.h"
#include "Engine.h"

void Mesh::Init(const vector<Vertex>& vertexBuffer, const vector<UINT32>& indexBuffer)
{

	//주어진 정점 버퍼 데이터를 사용하여 정점 버퍼 생성
	CreateVertexBuffer(vertexBuffer);
	//주어진 인덱스 버퍼 데이터를 사용하여 인덱스 버퍼 생성
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
	//인덱스 갯수 설정
	indexCount = static_cast<int>(buffer.size());
	//인덱스 버퍼의 크기(바이트 단위) 계산
	int bufferSize = indexCount * sizeof(UINT32);

	//힙 속성 설정 (업로드 힙)
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	//리소스 설명자 설정(버퍼)
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	//커밋된 리소스 생성
	Engine::Get().GetDevice()->GetDevice()->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuffer));

	//인덱스 데이터를 버퍼에 복사
	void* indexDataBuffer = nullptr;
	CD3DX12_RANGE readRange(0, 0); //읽기 범위 없음
	indexBuffer->Map(0, &readRange, &indexDataBuffer); // 버퍼 매핑
	memcpy(indexDataBuffer, &buffer[0], bufferSize);   //데이터 복사
	indexBuffer->Unmap(0, nullptr);// 버퍼 언맵


	//인덱스 버퍼 뷰 설정
	indexBufferView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;  // 인덱스 포맷 설정(32비트 정수)
	indexBufferView.SizeInBytes = bufferSize;		// 버퍼 크기 설정
}

void Mesh::Render()
{
	Engine::Get().GetCmdQueue()->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Engine::Get().GetCmdQueue()->GetCmdList()->IASetVertexBuffers(0, 1, &vertexBufferView);

	//인덱스 버퍼 바인딩
	Engine::Get().GetCmdQueue()->GetCmdList()->IASetIndexBuffer(&indexBufferView);

	Engine::Get().GetConstBuffer()->PushData(&transform, sizeof(transform));

	Engine::Get().GetTableDesc()->CommitTable();

	//인덱스를 사용하여 그리기(DrawIndexedInstanced)
	Engine::Get().GetCmdQueue()->GetCmdList()->DrawIndexedInstanced(indexCount, 1, 0, 0, 0);

}
