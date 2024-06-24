#include "pch.h"
#include "RootSignature.h"

void RootSignature::Init(ComPtr<ID3D12Device> device)
{
	//상수 버퍼 뷰(CBV) 레이지 배열 정의
	CD3DX12_DESCRIPTOR_RANGE ranges[] =
	{
		//CBV 레인지 타입, 레지스터 갯수, 기본 쉐이더 레지스터
		CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, CBV_REGISTER_COUNT, 0)
	};
	
	//루트 파라미터 배열 정의
	CD3DX12_ROOT_PARAMETER params[1]{};

	
	//params[0].InitAsConstantBufferView(0);  // 0번 -> b0

	//파라미터 디스크립터 테이블로 초기화
	//_countof(ranges) : 배열의 갯수
	params[0].InitAsDescriptorTable(_countof(ranges), ranges);


	//루트 시그니처 설명자 생성, 하나의 파라미터 사용
	D3D12_ROOT_SIGNATURE_DESC sigDesc = CD3DX12_ROOT_SIGNATURE_DESC(_countof(ranges), params);

	sigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	ComPtr<ID3DBlob> blobSignature; 
	ComPtr<ID3DBlob> blobError;

	D3D12SerializeRootSignature(&sigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &blobSignature, &blobError);
	device->CreateRootSignature(0, blobSignature->GetBufferPointer(), blobSignature->GetBufferSize(), IID_PPV_ARGS(&signature));
}
