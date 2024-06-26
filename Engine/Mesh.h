#pragma once
class Mesh
{
private:
	ComPtr<ID3D12Resource> vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView = {};
	UINT32 vertexCount = 0;

	ComPtr<ID3D12Resource> indexBuffer;		
	D3D12_INDEX_BUFFER_VIEW indexBufferView;
	UINT32 indexCount = 0;				

	XMFLOAT4 transform = {};
	//Texture 스마트 포인터
	shared_ptr<class Texture> texture = {};
public:
	void Init(const vector<Vertex>& vertexBuffer, const vector<UINT32>& indexBuffer);
	void Render();

	void SetTransform(const XMFLOAT4& _transform) { transform = _transform; }
	//Texture Set 함수
	void SetTexture(shared_ptr<class Texture> _texture) { texture = _texture; }

private:
	void CreateVertexBuffer(const vector<Vertex>& buffer);
	void CreateIndexBuffer(const vector<UINT32>& buffer);
	
};

