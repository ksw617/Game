#include "pch.h"
#include "RootSignature.h"

void RootSignature::Init(ComPtr<ID3D12Device> device)
{
	//호출
	CreateSamplerDesc();

	CD3DX12_DESCRIPTOR_RANGE ranges[] =
	{
		CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, CBV_REGISTER_COUNT, 0),
		//SRV 레인지 타입, 레지스터 갯수, 기본 쉐이더 레지스터
		CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, SRV_REGISTER_COUNT, 0)

	};
	
	CD3DX12_ROOT_PARAMETER params[1]{};

	params[0].InitAsDescriptorTable(_countof(ranges), ranges);

	//변경
	D3D12_ROOT_SIGNATURE_DESC sigDesc = CD3DX12_ROOT_SIGNATURE_DESC(_countof(params), params, 1, &samplerDesc);

	sigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	ComPtr<ID3DBlob> blobSignature; 
	ComPtr<ID3DBlob> blobError;

	D3D12SerializeRootSignature(&sigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &blobSignature, &blobError);
	device->CreateRootSignature(0, blobSignature->GetBufferPointer(), blobSignature->GetBufferSize(), IID_PPV_ARGS(&signature));
}

void RootSignature::CreateSamplerDesc()
{
	// CD3DX12_STATIC_SAMPLER_DESC(0)는 기본 샘플러 설명자를 생성하며,
	//레지스터 슬롯 0에 바인딩됨
	samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);
}
