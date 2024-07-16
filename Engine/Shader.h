#pragma once
#include "Object.h"

//Object 상속
class Shader : public Object
{
private:
	ComPtr<ID3DBlob> vsBlob; 
	ComPtr<ID3DBlob> psBlob; 
	ComPtr<ID3DBlob> errBlob;

	ComPtr<ID3D12PipelineState> pipelineState;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc = {}; 
public:
	//타입 초기화
	Shader() : Object(OBJECT_TYPE::SHADER) {}
	virtual ~Shader() {}
private:
	void CreateShader(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob, D3D12_SHADER_BYTECODE& shaderByteCode);
	void CreateVertexShader(const wstring& path, const string& name, const string& version);
	void CreatePixelShader(const wstring& path, const string& name, const string& version);
public:
	void Init(const wstring& path);
	void Update();
	
};

