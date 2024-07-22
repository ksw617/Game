#include "pch.h"
#include "RootSignature.h"

void RootSignature::Init(ComPtr<ID3D12Device> device)
{
	//호출
	CreateSamplerDesc();

	CD3DX12_DESCRIPTOR_RANGE ranges[] =
	{
		CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, CBV_REGISTER_COUNT -1, 1), //b1~b4에 대한 CBV 범위 설정
		//SRV 레인지 타입, 레지스터 갯수, 기본 쉐이더 레지스터
		CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, SRV_REGISTER_COUNT, 0)

	};
	
	//루트 파라미터 배열 초기화(2개로 변경)
	CD3DX12_ROOT_PARAMETER params[2]{};
	//b0에 대한 상수 버퍼 뷰로 초기화
	params[0].InitAsConstantBufferView(static_cast<UINT32>(CBV_REGISTER::b0));
	//디스크립터 테이블로 초기화 ranges 사용(b1~b4 & t0~t4)
	params[1].InitAsDescriptorTable(_countof(ranges), ranges);

	D3D12_ROOT_SIGNATURE_DESC sigDesc = CD3DX12_ROOT_SIGNATURE_DESC(_countof(params), params, 1, &samplerDesc);

	sigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	ComPtr<ID3DBlob> blobSignature; 
	ComPtr<ID3DBlob> blobError;

	D3D12SerializeRootSignature(&sigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &blobSignature, &blobError);
	device->CreateRootSignature(0, blobSignature->GetBufferPointer(), blobSignature->GetBufferSize(), IID_PPV_ARGS(&signature));
}

void RootSignature::CreateSamplerDesc()
{
	samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);
}
