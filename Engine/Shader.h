#pragma once
#include "Object.h"

//���̴� Ÿ��
enum class SHADER_TYPE : UINT8
{
	DEFERRED,
	FORWARD,
};


//�����Ͷ����� ������ �����ϴ� ������
enum class RASTERIZER_TYPE
{
	CULL_NONE,		
	CULL_FRONT,		
	CULL_BACK,		
	WIREFRAME,		

};

//���� ���ٽ� �׽�Ʈ ������ �����ϴ� ������
enum class DEPTH_STENCIL_TYPE
{
	LESS,			
	LESS_EQUAL,		
	GREATER,		
	GREATER_EQUAL,	

};

//���̴� ������ ��� �ִ� ����ü
struct ShaderInfo
{
	//���̴� Ÿ�� ����
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
	//�ʱ�ȭ
	void Init(const wstring& path, ShaderInfo info = ShaderInfo());
	void Update();
	
};

