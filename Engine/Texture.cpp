#include "pch.h"
#include "Texture.h"
#include "Engine.h"

void Texture::Init(const wstring& path)
{
	//텍스처 생성
	CreateTexture(path);

	//텍스처 뷰 생성
	CreateView();
}

void Texture::CreateTexture(const wstring& path)
{
	//파일 확장자 얻기
	wstring	ext = filesystem::path(path).extension();
										 
	//파일 확장자에 따라 적절한 로드 함수 호출
	if (ext == L".dds" || ext == L".DDS")
		LoadFromDDSFile(path.c_str(), DDS_FLAGS_NONE, nullptr, image);
	else if (ext == L".tga" || ext == L".TGA")
		LoadFromTGAFile(path.c_str(), nullptr, image);
	else // png, jpg, jpeg, bmp
		LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, nullptr, image);

	//텍스처 생성
	HRESULT hr = DirectX::CreateTexture(Engine::Get().GetDevice()->GetDevice().Get(), image.GetMetadata(), &texture2D);
	
	//못하면 크래쉬
	if (FAILED(hr))
		assert(nullptr);

	//서브 리소스 데이터 벡터
	vector<D3D12_SUBRESOURCE_DATA> subResource;

	//업로드 준비
	hr = PrepareUpload(Engine::Get().GetDevice()->GetDevice().Get(),
		image.GetImages(),
		image.GetImageCount(),
		image.GetMetadata(),
		subResource);

	//못하면 크래쉬
	if (FAILED(hr))
		assert(nullptr);

	//중간 버퍼의 크기 계산
	const UINT64 bufferSize = GetRequiredIntermediateSize(texture2D.Get(), 0, static_cast<UINT32>(subResource.size()));

	//힙 속성 설정(업로드 힙)
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	//리소스 설명자 설정(버퍼)
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	//텍스처 업로드 힙 리소스 생성
	ComPtr<ID3D12Resource> textureUploadHeap;
	hr = Engine::Get().GetDevice()->GetDevice()->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(textureUploadHeap.GetAddressOf()));

	//못하면 크래쉬
	if (FAILED(hr))
		assert(nullptr);

	//서브 리소스 텍스처에 복사
	UpdateSubresources(Engine::Get().GetCmdQueue()->GetResourceCmdList().Get(),
		texture2D.Get(),
		textureUploadHeap.Get(),
		0, 0,
		static_cast<UINT32>(subResource.size()),
		subResource.data());

	//리소스 명령 대기열 제출
	Engine::Get().GetCmdQueue()->SubmitResourceCommandQueue();
}

void Texture::CreateView()
{
	//SRV 디스크립터 힙 설명자 초기화
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.NumDescriptors = 1;	//디스크립터 수는 1개
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV; //힙 타입은 CBV/SRV/UAV
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE; // 플래그 없음
	//디스크립터 힙 생성
	Engine::Get().GetDevice()->GetDevice()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));

	//SRV의 CPU 디스크립터 핸들 가져오기
	srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

	//쉐이더 리소스 뷰(SRV) 설명자 초기화
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = image.GetMetadata().format; // 이미지 포맷 설정
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2D 텍스쳐 뷰 설정
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING; // 기본 쉐이더 4 컴포넌트 제공
	srvDesc.Texture2D.MipLevels = 1; //Mip 레벨수 설정
	Engine::Get().GetDevice()->GetDevice()->CreateShaderResourceView(texture2D.Get(), &srvDesc, srvHandle); // SRV 생성
}
