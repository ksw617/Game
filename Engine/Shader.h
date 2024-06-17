#pragma once
class Shader
{
private:
	ComPtr<ID3DBlob> vsBlob; // 정점 쉐이더 blob
	ComPtr<ID3DBlob> psBlob; // 픽셀 쉐이더 blob
	ComPtr<ID3DBlob> errBlob;// 에러 blob

	ComPtr<ID3D12PipelineState> pipelineState; // 파이프라인 상태 객체
	//그래픽 파이프라인 상태 설명자
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc = {}; 
private:
	//주어진 경로와 설정으로 쉐이더 생성
	void CreateShader(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob, D3D12_SHADER_BYTECODE& shaderByteCode);

	//주어진 경로와 설정으로 정점 쉐이더 생성
	void CreateVertexShader(const wstring& path, const string& name, const string& version);
	
	//주어진 경로와 설정으로 픽셀 쉐이더 생성
	void CreatePixelShader(const wstring& path, const string& name, const string& version);
public:
	// 쉐이더를 초기화(주어진 경로의 쉐이더 파일을 사용)
	void Init(const wstring& path);
	//쉐이더 상태를 업데이트
	void Update();
	
};

