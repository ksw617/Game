#pragma once
class RootSignature
{
private:
	//루트 시그니처 객체를 관리하는 스마트 포인터
	ComPtr<ID3D12RootSignature> signature;
public:
	//루트 시그니처를 반환하는 Get 함수
	ComPtr<ID3D12RootSignature> GetSignature() { return signature; }

public:
	//주어진 디바이스를 사용하여 루트 시그니처를 초기화
	void Init(ComPtr<ID3D12Device> device);

};

