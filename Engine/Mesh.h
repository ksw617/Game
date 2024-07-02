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

	//제거
	//XMFLOAT4 transform = {};
	//shared_ptr<class Material> material = {};
public:
	void Init(const vector<Vertex>& vertexBuffer, const vector<UINT32>& indexBuffer);
	void Render();

	//제거
	//void SetTransform(const XMFLOAT4& _transform) { transform = _transform; }
	//void SetMaterial(shared_ptr<class Material> mat) { material = mat; }

private:
	void CreateVertexBuffer(const vector<Vertex>& buffer);
	void CreateIndexBuffer(const vector<UINT32>& buffer);
	
};

