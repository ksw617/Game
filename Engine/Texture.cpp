#include "pch.h"
#include "Texture.h"
#include "Engine.h"

void Texture::Load(const wstring& path)
{
	//�ؽ�ó ����
	CreateTexture(path);

	//�ؽ�ó �� ����
	CreateView();
}

void Texture::Create(DXGI_FORMAT format, UINT32 width, UINT32 height, const D3D12_HEAP_PROPERTIES& heapProperty, D3D12_HEAP_FLAGS heapFlags, D3D12_RESOURCE_FLAGS resFlags, Vector4 clearColor)
{
	// �ؽ�ó 2D ���ҽ� �����ڸ� �����ϰ� �ʱ�ȭ
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Tex2D(format, width, height);
	desc.Flags = resFlags; // ���޹��� ���ҽ� �÷��׸� ����

	// ����ȭ�� Ŭ���� ���� ���ҽ� ���¸� �ʱ�ȭ
	D3D12_CLEAR_VALUE optimizedClearValue = {}; // Ŭ���� ���� �⺻������ �ʱ�ȭ
	D3D12_RESOURCE_STATES resourceStates = D3D12_RESOURCE_STATE_COMMON; // �⺻ ���ҽ� ���¸� �Ϲ� ���·� ����

	// ���ҽ� �÷��׿� ���� �߰� �ʱ�ȭ
	if (resFlags & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL)
	{
		// ���� ���ٽ��� ����ϴ� ���
		resourceStates = D3D12_RESOURCE_STATE_DEPTH_WRITE; // ���ҽ� ���¸� ���� ����� ����
		optimizedClearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0); // ���� ���� ���ٽ� ���� ����
	}   
	// ���� Ÿ���� ����ϴ� ���
	else if (resFlags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET)
	{
		resourceStates = D3D12_RESOURCE_STATE_RENDER_TARGET; // ���ҽ� ���¸� ���� Ÿ������ ����
		float arrFloat[4] = { clearColor.x, clearColor.y, clearColor.z, clearColor.w }; // Ŭ���� ���� �迭 ����
		optimizedClearValue = CD3DX12_CLEAR_VALUE(format, arrFloat); // Ŭ���� ���� ����
	}

	// �ؽ�ó 2D ���ҽ��� ����
	HRESULT hr = Engine::Get().GetDevice()->GetDevice()->CreateCommittedResource(
		&heapProperty, // �� �Ӽ�
		heapFlags, // �� �÷���
		&desc, // ���ҽ� ������
		resourceStates, // �ʱ� ���ҽ� ����
		&optimizedClearValue, // ����ȭ�� Ŭ���� ��
		IID_PPV_ARGS(&texture2D) // ������ ���ҽ��� texture2D ��� ������ ����
	);

	// ���ҽ� ������ �����ߴ��� Ȯ��
	assert(SUCCEEDED(hr)); // ���� �� assert�� ���α׷� ����

	// ������ ���ҽ��κ��� �ؽ�ó �並 ����
	CreateFromResource(texture2D); // ���ҽ��� ������� �並 �����ϴ� �Լ� ȣ��

}

void Texture::CreateFromResource(ComPtr<ID3D12Resource> tex2D)
{
	texture2D = tex2D; // ���޵� ���ҽ� �����͸� Ŭ���� ��� ������ ����

	D3D12_RESOURCE_DESC desc = texture2D->GetDesc(); // ���ҽ� ������ ��������

	// �ؽ�ó ���ҽ��� �÷��׿� ���� �並 �����Ѵ�
	 // DSV : ���� ���ٽ� ��
	 // SRV : ���̴� ���ҽ� ��
	 // RTV + SRV : ���� Ÿ�� �� + ���̴� ���ҽ� ��
	if (desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL)
	{
		 //DSV
		 // ���� ���ٽ� ��(DSV) ����
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV; // �� Ÿ�� ����
		heapDesc.NumDescriptors = 1; // ��ũ���� �� ����
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE; // �÷��� ����
		heapDesc.NodeMask = 0; // ��� ����ũ ���� (��Ƽ ����Ϳ�)

		// DSV �� ����
		Engine::Get().GetDevice()->GetDevice()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&dsvHeap));

		//���� / ���ٽ� ���� CPU ��ũ���� �ڵ� ��������
		D3D12_CPU_DESCRIPTOR_HANDLE hDSVHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();
		//���� / ���ٽ� �� ����
		Engine::Get().GetDevice()->GetDevice()->CreateDepthStencilView(texture2D.Get(), nullptr, hDSVHandle);

	}
	else
	{
		if (desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET)
		{
			//RTV
			// ���� Ÿ�� ��(RTV) ����
			D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
			heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // �� Ÿ�� ����
			heapDesc.NumDescriptors = 1; // ��ũ���� �� ����
			heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE; // �÷��� ����
			heapDesc.NodeMask = 0; // ��� ����ũ ���� (��Ƽ ����Ϳ�)

			// RTV �� ����
			Engine::Get().GetDevice()->GetDevice()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeap));

			// RTV �ڵ� ����
			D3D12_CPU_DESCRIPTOR_HANDLE rtvHeapBegin = rtvHeap->GetCPUDescriptorHandleForHeapStart();
			Engine::Get().GetDevice()->GetDevice()->CreateRenderTargetView(texture2D.Get(), nullptr, rtvHeapBegin);


		}

		//SRV
		  // ���̴� ���ҽ� ��(SRV) ����
		D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
		srvHeapDesc.NumDescriptors = 1; // ��ũ���� �� ����
		srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV; // �� Ÿ�� ����
		srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE; // �÷��� ����

		// SRV �� ����
		Engine::Get().GetDevice()->GetDevice()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));

		srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart(); // SRV �ڵ� ����

		// SRV ������ ����
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING; // ���̴� 4 ������Ʈ ���� ����
		srvDesc.Format = image.GetMetadata().format; // �ؽ�ó ���� ����
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // SRV ���� ����
		srvDesc.Texture2D.MipLevels = 1; // Mip ���� ����
		Engine::Get().GetDevice()->GetDevice()->CreateShaderResourceView(texture2D.Get(), &srvDesc, srvHandle); // SRV ����

	}
}

void Texture::CreateTexture(const wstring& path)
{
	//���� Ȯ���� ���
	wstring	ext = filesystem::path(path).extension();
										 
	//���� Ȯ���ڿ� ���� ������ �ε� �Լ� ȣ��
	if (ext == L".dds" || ext == L".DDS")
		LoadFromDDSFile(path.c_str(), DDS_FLAGS_NONE, nullptr, image);
	else if (ext == L".tga" || ext == L".TGA")
		LoadFromTGAFile(path.c_str(), nullptr, image);
	else // png, jpg, jpeg, bmp
		LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, nullptr, image);

	//�ؽ�ó ����
	HRESULT hr = DirectX::CreateTexture(Engine::Get().GetDevice()->GetDevice().Get(), image.GetMetadata(), &texture2D);
	
	//���ϸ� ũ����
	if (FAILED(hr))
		assert(nullptr);

	//���� ���ҽ� ������ ����
	vector<D3D12_SUBRESOURCE_DATA> subResource;

	//���ε� �غ�
	hr = PrepareUpload(Engine::Get().GetDevice()->GetDevice().Get(),
		image.GetImages(),
		image.GetImageCount(),
		image.GetMetadata(),
		subResource);

	//���ϸ� ũ����
	if (FAILED(hr))
		assert(nullptr);

	//�߰� ������ ũ�� ���
	const UINT64 bufferSize = GetRequiredIntermediateSize(texture2D.Get(), 0, static_cast<UINT32>(subResource.size()));

	//�� �Ӽ� ����(���ε� ��)
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	//���ҽ� ������ ����(����)
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	//�ؽ�ó ���ε� �� ���ҽ� ����
	ComPtr<ID3D12Resource> textureUploadHeap;
	hr = Engine::Get().GetDevice()->GetDevice()->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(textureUploadHeap.GetAddressOf()));

	//���ϸ� ũ����
	if (FAILED(hr))
		assert(nullptr);

	//���� ���ҽ� �ؽ�ó�� ����
	UpdateSubresources(Engine::Get().GetCmdQueue()->GetResourceCmdList().Get(),
		texture2D.Get(),
		textureUploadHeap.Get(),
		0, 0,
		static_cast<UINT32>(subResource.size()),
		subResource.data());

	//���ҽ� ��� ��⿭ ����
	Engine::Get().GetCmdQueue()->SubmitResourceCommandQueue();
}

void Texture::CreateView()
{
	//SRV ��ũ���� �� ������ �ʱ�ȭ
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.NumDescriptors = 1;	//��ũ���� ���� 1��
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV; //�� Ÿ���� CBV/SRV/UAV
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE; // �÷��� ����
	//��ũ���� �� ����
	Engine::Get().GetDevice()->GetDevice()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));

	//SRV�� CPU ��ũ���� �ڵ� ��������
	srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

	//���̴� ���ҽ� ��(SRV) ������ �ʱ�ȭ
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = image.GetMetadata().format; // �̹��� ���� ����
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2D �ؽ��� �� ����
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING; // �⺻ ���̴� 4 ������Ʈ ����
	srvDesc.Texture2D.MipLevels = 1; //Mip ������ ����
	Engine::Get().GetDevice()->GetDevice()->CreateShaderResourceView(texture2D.Get(), &srvDesc, srvHandle); // SRV ����
}
