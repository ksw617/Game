#pragma once
#include "Object.h"

//래스터라이저 유형을 정의하는 열거형
enum class RASTERRIZER_TYPE
{
	CULL_NONE,		//컬링없음
	CULL_FRONT,		//앞면 컬링
	CULL_BACK,		//뒷면 컬링
	WIREFRAME,		//와이어 프레임 모드

};

//깊이 스텐실 테스트 유형을 정의하는 열거형
enum class DEPTH_STENCIL_TYPE
{
	LESS,			//깊이 값이 작을 때 통과
	LESS_EQUAL,		//깊이 값이 작거나 같을 때 통과
	GREATER,		//깊이 값이 클때 통과
	GREATER_EQUAL,	//깊이 값이 크거나 같을 때 통과

};

//쉐이더 정보를 담고 있는 구조체
struct ShaderInfo
{
	//기본적으로 뒷면 컬링을 사용
	RASTERRIZER_TYPE rasterrizerType = RASTERRIZER_TYPE::CULL_BACK;

	//기본적으로 깊이 값이 적을때 통과
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

