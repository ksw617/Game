#include "pch.h"
#include "RootSignature.h"

void RootSignature::Init(ComPtr<ID3D12Device> device)
{
	//두개의 루트 파라미터 배열 생성
	CD3DX12_ROOT_PARAMETER params[2];

	//첫번째 루트 파라미터를 상수 버퍼 뷰로 초기화, 레지스터 b0에 바이딩
	params[0].InitAsConstantBufferView(0);  // 0번 -> b0

	//두번째 루트 파라미터를 상수 버퍼 뷰로 초기화, 레지스터 b1에 바이딩
	params[1].InitAsConstantBufferView(1);	// 1번 -> b1


	//루트 시그니처 설명자 생성, 두개의 루트 파라미터 사용
	D3D12_ROOT_SIGNATURE_DESC sigDesc = CD3DX12_ROOT_SIGNATURE_DESC(2, params);
	sigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	ComPtr<ID3DBlob> blobSignature; 
	ComPtr<ID3DBlob> blobError;

	D3D12SerializeRootSignature(&sigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &blobSignature, &blobError);
	device->CreateRootSignature(0, blobSignature->GetBufferPointer(), blobSignature->GetBufferSize(), IID_PPV_ARGS(&signature));
}
