#pragma once
#include "Object.h"

enum class SHADER_TYPE : UINT8
{
	DEFERRED,
	FORWARD,
};

enum class RASTERIZER_TYPE
{
	CULL_NONE,		
	CULL_FRONT,		
	CULL_BACK,		
	WIREFRAME,		

};

enum class DEPTH_STENCIL_TYPE
{
	LESS,			
	LESS_EQUAL,		
	GREATER,		
	GREATER_EQUAL,	

};

struct ShaderInfo
{
	SHADER_TYPE shaderType = SHADER_TYPE::FORWARD;
	RASTERIZER_TYPE rasterrizerType = RASTERIZER_TYPE::CULL_BACK;
	DEPTH_STENCIL_TYPE depthStencilType = DEPTH_STENCIL_TYPE::LESS;
	//토폴로지도 추가
	D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
};

class Shader : public Object
{
private:
	//쉐이더 정보 들고 있게
	ShaderInfo shaderInfo;
	ComPtr<ID3DBlob> vsBlob; 
	ComPtr<ID3DBlob> psBlob; 
	ComPtr<ID3DBlob> errBlob;

	ComPtr<ID3D12PipelineState> pipelineState;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc = {}; 
public:
	Shader() : Object(OBJECT_TYPE::SHADER) {}
	virtual ~Shader() {}
public:
	//쉐이더 타입 Get 함수
	SHADER_TYPE GetShaderType() const { return shaderInfo.shaderType; }
private:
	void CreateShader(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob, D3D12_SHADER_BYTECODE& shaderByteCode);
	void CreateVertexShader(const wstring& path, const string& name, const string& version);
	void CreatePixelShader(const wstring& path, const string& name, const string& version);
public:

	void Init(const wstring& path, ShaderInfo info = ShaderInfo());
	void Update();
	
};

