#include "pch.h"
#include "Mesh.h"
#include "Engine.h"

void Mesh::Init(vector<Vertex>& vec)
{
	//정점의 개수를 저장
	vertexCount = static_cast<int>(vec.size());
	//버퍼 크기 계산
	int bufferSize = vertexCount * sizeof(Vertex);

	//힙 속성 설정(업로드 힙)
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	//리소스 설명자 설정 (버퍼) 
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	//커밋 리소스 생성
	Engine::Get().GetDevice()->GetDevice()->CreateCommittedResource(
	  &heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertexBuffer));

	//정점 데이터를 버퍼에 복사
	void* vertexDataBuffer = nullptr;
	CD3DX12_RANGE readRange(0,0);// 읽기 범위 없음
	vertexBuffer->Map(0, &readRange, &vertexDataBuffer);
	memcpy(vertexDataBuffer, &vec[0], bufferSize);
	vertexBuffer->Unmap(0, nullptr);

	//정점 버퍼 뷰 설정
	vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
	vertexBufferView.StrideInBytes = sizeof(Vertex);
	vertexBufferView.SizeInBytes = bufferSize;
	
}

//메쉬를 랜더링
void Mesh::Render()
{
	//기본 프리미티브 토폴로지를 삼각형 리스트로 설정
	Engine::Get().GetCmdQueue()->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//정점 버퍼를 슬롯 0에 설정 (슬롯 : 0~15)
	Engine::Get().GetCmdQueue()->GetCmdList()->IASetVertexBuffers(0, 1, &vertexBufferView);
	//인스턴스를 그리기
	Engine::Get().GetCmdQueue()->GetCmdList()->DrawInstanced(vertexCount, 1, 0, 0);

}
