#include "pch.h"
#include "Shader.h"
#include "Engine.h"


void Shader::Init(const wstring& path)
{
	//정점 쉐이더 생성
	CreateVertexShader(path, "VS_Main", "vs_5_0");

	//픽셀 쉐이더 생성
	CreatePixelShader(path, "PS_Main", "ps_5_0");

	//입력 요소 설명자 설정(정점 데이터 레이아웃)
	D3D12_INPUT_ELEMENT_DESC desc[] = {

		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};

	//파이프라인 상태 객체 (PSO) 설명자 설정
	pipelineDesc.InputLayout = { desc, _countof(desc) };
	pipelineDesc.pRootSignature = Engine::Get().GetRootSignature()->GetSignature().Get();

	pipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	pipelineDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	pipelineDesc.DepthStencilState.DepthEnable = FALSE;
	pipelineDesc.DepthStencilState.StencilEnable = FALSE;
	pipelineDesc.SampleMask = UINT_MAX;
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	pipelineDesc.NumRenderTargets = 1;
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8_UNORM;
	pipelineDesc.SampleDesc.Count = 1;

	//그래픽스 파이프 라인 상태 생성
	Engine::Get().GetDevice()->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));

}

void Shader::Update()
{
	//명령 목록에 파이프라인 상태 설정
	Engine::Get().GetCmdQueue()->GetCmdList()->SetPipelineState(pipelineState.Get());
}


void Shader::CreateShader(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob, D3D12_SHADER_BYTECODE& shaderByteCode)
{
	UINT32 compileFlag = 0;
#ifdef DEBUG
	compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif // DEBUG
		 
	//쉐이더 파일 컴파일
	if (FAILED(D3DCompileFromFile(path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, name.c_str(), version.c_str(), compileFlag, 0, &blob, &errBlob)))
	{
		//컴파일 실패 시 메세지 박스 표시
		MessageBoxA(nullptr, "Shader Create Failed!", nullptr, MB_OK);
	}

	//쉐이더 바이트 코드 설정
	shaderByteCode = { blob->GetBufferPointer(), blob->GetBufferSize() };
}

void Shader::CreateVertexShader(const wstring& path, const string& name, const string& version)
{
	//정점 쉐이더 생성
	CreateShader(path, name, version, vsBlob, pipelineDesc.VS);
}

void Shader::CreatePixelShader(const wstring& path, const string& name, const string& version)
{
	//픽셀 쉐이더 생성
	CreateShader(path, name, version, psBlob, pipelineDesc.PS);
}
