#include "pch.h"
#include "RootSignature.h"

//루트 시그니처 초기화
void RootSignature::Init(ComPtr<ID3D12Device> device)
{
	//기본 설정을 사용하여 루트 시그니처를 설명하고 입력 조립기 단계를 허용
	D3D12_ROOT_SIGNATURE_DESC sigDesc = CD3DX12_ROOT_SIGNATURE_DESC(D3D12_DEFAULT);
	sigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	//직렬화된 루트 시그니처를 저장하는 blob
	ComPtr<ID3DBlob> blobSignature; 
	//직렬화 실패 시 오류 메세지 저장하는 blob
	ComPtr<ID3DBlob> blobError;

	//루트 시그니처를 직렬화
	D3D12SerializeRootSignature(&sigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &blobSignature, &blobError);
	//직렬화된 데이터를 사용하여 루트 시그니처 생성
	device->CreateRootSignature(0, blobSignature->GetBufferPointer(), blobSignature->GetBufferSize(), IID_PPV_ARGS(&signature));
}
