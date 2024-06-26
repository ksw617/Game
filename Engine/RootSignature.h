#pragma once
class RootSignature
{
private:
	ComPtr<ID3D12RootSignature> signature;
	//샘플러 설명자
	D3D12_STATIC_SAMPLER_DESC samplerDesc;

public:
	ComPtr<ID3D12RootSignature> GetSignature() { return signature; }
public:
	void Init(ComPtr<ID3D12Device> device);

	//샘플러 설명자를 생성하는 함수
	void CreateSamplerDesc();

};

