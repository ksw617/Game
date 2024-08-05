#pragma once
#include "Object.h"

//쉐이더 타입
enum class SHADER_TYPE : UINT8
{
	DEFERRED,
	FORWARD,
};


//래스터라이저 유형을 정의하는 열거형
enum class RASTERIZER_TYPE
{
	CULL_NONE,		
	CULL_FRONT,		
	CULL_BACK,		
	WIREFRAME,		

};

//깊이 스텐실 테스트 유형을 정의하는 열거형
enum class DEPTH_STENCIL_TYPE
{
	LESS,			
	LESS_EQUAL,		
	GREATER,		
	GREATER_EQUAL,	

};

//쉐이더 정보를 담고 있는 구조체
struct ShaderInfo
{
	//쉐이더 타입 설정
	SHADER_TYPE shaderType = SHADER_TYPE::FORWARD;
	RASTERIZER_TYPE rasterrizerType = RASTERIZER_TYPE::CULL_BACK;
	DEPTH_STENCIL_TYPE depthStencilType = DEPTH_STENCIL_TYPE::LESS;
};

class Shader : public Object
{
private:
	ComPtr<ID3DBlob> vsBlob; 
	ComPtr<ID3DBlob> psBlob; 
	ComPtr<ID3DBlob> errBlob;

	ComPtr<ID3D12PipelineState> pipelineState;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc = {}; 
public:
	Shader() : Object(OBJECT_TYPE::SHADER) {}
	virtual ~Shader() {}
private:
	void CreateShader(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob, D3D12_SHADER_BYTECODE& shaderByteCode);
	void CreateVertexShader(const wstring& path, const string& name, const string& version);
	void CreatePixelShader(const wstring& path, const string& name, const string& version);
public:
	//초기화
	void Init(const wstring& path, ShaderInfo info = ShaderInfo());
	void Update();
	
};

