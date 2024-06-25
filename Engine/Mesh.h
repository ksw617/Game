#pragma once
class Mesh
{
private:
	ComPtr<ID3D12Resource> vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView = {};
	UINT32 vertexCount = 0;

	ComPtr<ID3D12Resource> indexBuffer;		//인덱스 버퍼 리소스
	D3D12_INDEX_BUFFER_VIEW indexBufferView;//인덱스 버퍼 뷰
	UINT32 indexCount = 0;					//인덱스 갯수

	XMFLOAT4 transform = {};
public:
	//정점 버퍼와 인덱스 버퍼를 초기화
	void Init(const vector<Vertex>& vertexBuffer, const vector<UINT32>& indexBuffer);
	void Render();

	void SetTransform(const XMFLOAT4& _transform) { transform = _transform; }

private:
	//정점 버퍼 생성
	void CreateVertexBuffer(const vector<Vertex>& buffer);
	//인덱스 버퍼 생성
	void CreateIndexBuffer(const vector<UINT32>& buffer);
	
};

