#include "pch.h"
#include "Texture.h"
#include "Engine.h"

void Texture::Load(const wstring& path)
{
	//텍스처 생성
	CreateTexture(path);

	//텍스처 뷰 생성
	CreateView();
}

void Texture::Create(DXGI_FORMAT format, UINT32 width, UINT32 height, const D3D12_HEAP_PROPERTIES& heapProperty, D3D12_HEAP_FLAGS heapFlags, D3D12_RESOURCE_FLAGS resFlags, Vector4 clearColor)
{
	// 텍스처 2D 리소스 설명자를 생성하고 초기화
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Tex2D(format, width, height);
	desc.Flags = resFlags; // 전달받은 리소스 플래그를 설정

	// 최적화된 클리어 값과 리소스 상태를 초기화
	D3D12_CLEAR_VALUE optimizedClearValue = {}; // 클리어 값을 기본값으로 초기화
	D3D12_RESOURCE_STATES resourceStates = D3D12_RESOURCE_STATE_COMMON; // 기본 리소스 상태를 일반 상태로 설정

	// 리소스 플래그에 따라 추가 초기화
	if (resFlags & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL)
	{
		// 깊이 스텐실을 허용하는 경우
		resourceStates = D3D12_RESOURCE_STATE_DEPTH_WRITE; // 리소스 상태를 깊이 쓰기로 설정
		optimizedClearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0); // 깊이 값과 스텐실 값을 설정
	}   
	// 렌더 타겟을 허용하는 경우
	else if (resFlags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET)
	{
		resourceStates = D3D12_RESOURCE_STATE_RENDER_TARGET; // 리소스 상태를 렌더 타겟으로 설정
		float arrFloat[4] = { clearColor.x, clearColor.y, clearColor.z, clearColor.w }; // 클리어 색상 배열 생성
		optimizedClearValue = CD3DX12_CLEAR_VALUE(format, arrFloat); // 클리어 값을 설정
	}

	// 텍스처 2D 리소스를 생성
	HRESULT hr = Engine::Get().GetDevice()->GetDevice()->CreateCommittedResource(
		&heapProperty, // 힙 속성
		heapFlags, // 힙 플래그
		&desc, // 리소스 설명자
		resourceStates, // 초기 리소스 상태
		&optimizedClearValue, // 최적화된 클리어 값
		IID_PPV_ARGS(&texture2D) // 생성된 리소스를 texture2D 멤버 변수에 저장
	);

	// 리소스 생성이 성공했는지 확인
	assert(SUCCEEDED(hr)); // 실패 시 assert로 프로그램 종료

	// 생성된 리소스로부터 텍스처 뷰를 생성
	CreateFromResource(texture2D); // 리소스를 기반으로 뷰를 생성하는 함수 호출

}

void Texture::CreateFromResource(ComPtr<ID3D12Resource> tex2D)
{
	texture2D = tex2D; // 전달된 리소스 포인터를 클래스 멤버 변수에 저장

	D3D12_RESOURCE_DESC desc = texture2D->GetDesc(); // 리소스 설명자 가져오기

	// 텍스처 리소스의 플래그에 따라 뷰를 생성한다
	 // DSV : 깊이 스텐실 뷰
	 // SRV : 쉐이더 리소스 뷰
	 // RTV + SRV : 렌더 타겟 뷰 + 쉐이더 리소스 뷰
	if (desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL)
	{
		 //DSV
		 // 깊이 스텐실 뷰(DSV) 생성
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV; // 힙 타입 설정
		heapDesc.NumDescriptors = 1; // 디스크립터 수 설정
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE; // 플래그 설정
		heapDesc.NodeMask = 0; // 노드 마스크 설정 (멀티 어댑터용)

		// DSV 힙 생성
		Engine::Get().GetDevice()->GetDevice()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&dsvHeap));

		//깊이 / 스텐실 뷰의 CPU 디스크립터 핸들 가져오기
		D3D12_CPU_DESCRIPTOR_HANDLE hDSVHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();
		//깊이 / 스텐실 뷰 생성
		Engine::Get().GetDevice()->GetDevice()->CreateDepthStencilView(texture2D.Get(), nullptr, hDSVHandle);

	}
	else
	{
		if (desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET)
		{
			//RTV
			// 렌더 타겟 뷰(RTV) 생성
			D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
			heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // 힙 타입 설정
			heapDesc.NumDescriptors = 1; // 디스크립터 수 설정
			heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE; // 플래그 설정
			heapDesc.NodeMask = 0; // 노드 마스크 설정 (멀티 어댑터용)

			// RTV 힙 생성
			Engine::Get().GetDevice()->GetDevice()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeap));

			// RTV 핸들 생성
			D3D12_CPU_DESCRIPTOR_HANDLE rtvHeapBegin = rtvHeap->GetCPUDescriptorHandleForHeapStart();
			Engine::Get().GetDevice()->GetDevice()->CreateRenderTargetView(texture2D.Get(), nullptr, rtvHeapBegin);


		}

		//SRV
		  // 쉐이더 리소스 뷰(SRV) 생성
		D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
		srvHeapDesc.NumDescriptors = 1; // 디스크립터 수 설정
		srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV; // 힙 타입 설정
		srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE; // 플래그 설정

		// SRV 힙 생성
		Engine::Get().GetDevice()->GetDevice()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));

		srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart(); // SRV 핸들 저장

		// SRV 설명자 생성
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING; // 셰이더 4 컴포넌트 매핑 설정
		srvDesc.Format = image.GetMetadata().format; // 텍스처 형식 설정
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // SRV 차원 설정
		srvDesc.Texture2D.MipLevels = 1; // Mip 레벨 설정
		Engine::Get().GetDevice()->GetDevice()->CreateShaderResourceView(texture2D.Get(), &srvDesc, srvHandle); // SRV 생성

	}
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
