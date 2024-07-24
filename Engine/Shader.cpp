#include "pch.h"
#include "Shader.h"
#include "Engine.h"


void Shader::Init(const wstring& path, ShaderInfo info)
{
	CreateVertexShader(path, "VS_Main", "vs_5_0");
	CreatePixelShader(path, "PS_Main", "ps_5_0");

	D3D12_INPUT_ELEMENT_DESC desc[] = {

		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		//{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		//Normal & Tangent 추가
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};

	pipelineDesc.InputLayout = { desc, _countof(desc) };
	pipelineDesc.pRootSignature = Engine::Get().GetRootSignature()->GetSignature().Get();

	pipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	pipelineDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	pipelineDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	pipelineDesc.SampleMask = UINT_MAX;
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	pipelineDesc.NumRenderTargets = 1;
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8_UNORM;
	pipelineDesc.SampleDesc.Count = 1;

	
	pipelineDesc.DSVFormat = Engine::Get().GetDepthStencilBuffer()->GetDSVFormat();

	switch (info.rasterrizerType)		
	{
	case RASTERRIZER_TYPE::CULL_BACK:
		//뒷면 컬링 사용하고, 솔리드 모드로 설정
		pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
		break;
	case RASTERRIZER_TYPE::CULL_FRONT:
		//앞면 컬링 사용하고, 솔리드 모드로 설정
		pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_FRONT;
		break;
	case RASTERRIZER_TYPE::CULL_NONE:
		//컬링을 사용하지 않고, 솔리드 모드로 설정
		pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		break;
	case RASTERRIZER_TYPE::WIREFRAME:
		//컬링을 사용하지 않고, 와이어 프레임 모드로 설정
		pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
		pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		break;
	}

	//깊이 스텐실 설정을 위한 switch문
	switch (info.depthStencilType)
	{
	case DEPTH_STENCIL_TYPE::LESS:
		//깊이 테스트를 활성화하고, LESS 비교 함수를 사용
		pipelineDesc.DepthStencilState.DepthEnable = TRUE;
		pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		break;
	case DEPTH_STENCIL_TYPE::LESS_EQUAL:
		//깊이 테스트를 활성화하고, LESS_EQUAL 비교 함수를 사용
		pipelineDesc.DepthStencilState.DepthEnable = TRUE;
		pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		break;
	case DEPTH_STENCIL_TYPE::GREATER:
		//깊이 테스트를 활성화하고, GREATER 비교 함수를 사용
		pipelineDesc.DepthStencilState.DepthEnable = TRUE;
		pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_GREATER;
		break;
	case DEPTH_STENCIL_TYPE::GREATER_EQUAL:
		//깊이 테스트를 활성화하고, GREATER_EQUAL 비교 함수를 사용
		pipelineDesc.DepthStencilState.DepthEnable = TRUE;
		pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_GREATER_EQUAL;
		break;

	}



	Engine::Get().GetDevice()->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));

}

void Shader::Update()
{
	Engine::Get().GetCmdQueue()->GetCmdList()->SetPipelineState(pipelineState.Get());
}


void Shader::CreateShader(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob, D3D12_SHADER_BYTECODE& shaderByteCode)
{
	UINT32 compileFlag = 0;
#ifdef DEBUG
	compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif // DEBUG
		 
	if (FAILED(D3DCompileFromFile(path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, name.c_str(), version.c_str(), compileFlag, 0, &blob, &errBlob)))
	{
		MessageBoxA(nullptr, "Shader Create Failed!", nullptr, MB_OK);
	}

	shaderByteCode = { blob->GetBufferPointer(), blob->GetBufferSize() };
}

void Shader::CreateVertexShader(const wstring& path, const string& name, const string& version)
{
	CreateShader(path, name, version, vsBlob, pipelineDesc.VS);
}

void Shader::CreatePixelShader(const wstring& path, const string& name, const string& version)
{
	CreateShader(path, name, version, psBlob, pipelineDesc.PS);
}
