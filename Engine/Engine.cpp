#include "pch.h"
#include "Engine.h"
#include "Material.h" 
#include "SceneManager.h"
#include "Light.h" 
#include "Resources.h"

void Engine::Init(HWND _hwnd, int _width, int _height, bool _windowed)
{
	hwnd = _hwnd;
	width = _width;
	height = _height;
	windowed = _windowed;

				  
	viewPort = { 0,0, static_cast<FLOAT>(width), static_cast<FLOAT>(height), 0.0f, 1.0f };

	scissorRect = CD3DX12_RECT(0, 0, width, height);

	device = make_shared<Device>();
	cmdQueue = make_shared<CommandQueue>();
	swapChain = make_shared<SwapChain>();
	rootSignature = make_shared<RootSignature>();
	tableDesc = make_shared<TableDescriptor>();
	//depthStencilBuffer = make_shared<DepthStencilBuffer>(); 

	input = make_shared<Input>(); 
	timer = make_shared<Timer>();

	device->Init();
	cmdQueue->Init(device->GetDevice(), swapChain);
	swapChain->Init(hwnd, width, height, windowed, device->GetDevice(), device->GetDXGI(), cmdQueue->GetCmdQueue());
	rootSignature->Init(device->GetDevice());

	CreateConstantBuffer(CBV_REGISTER::b0, sizeof(LightParams), 1);
	CreateConstantBuffer(CBV_REGISTER::b1, sizeof(TransformParams), 256);
	CreateConstantBuffer(CBV_REGISTER::b2, sizeof(MaterialParams), 256);


	//CreateMultipleRenderTarget ȣ��
	CreateMultipleRenderTarget();

	tableDesc->Init(256);


	input->Init(hwnd);
	timer->Init();

	Resources::Get().Init();

	ResizeWindow(width, height);
}

void Engine::Render()
{
	RenderBegin();

	SceneManager::Get().Render(); 

	RenderEnd();
}

void Engine::Update()
{
	input->Update();
	timer->Update();

	SceneManager::Get().Update();

	Render();

	ShowFPS();
}

void Engine::ShowFPS()
{
	UINT32 fps = timer->GetFPS();
	wstring fpsText = L"FPS: " + to_wstring(fps);

	SetWindowText(hwnd, fpsText.c_str());
}

void Engine::CreateConstantBuffer(CBV_REGISTER reg, UINT32 bufferSize, UINT32 count)
{
	UINT8 typeInt = static_cast<UINT8>(reg);

	assert(constantBuffers.size() == typeInt);

	shared_ptr<ConstantBuffer> buffer = make_shared<ConstantBuffer>();
	buffer->Init(reg, bufferSize, count);

	constantBuffers.push_back(buffer);

}

// ���� ���� Ÿ���� �����ϴ� �Լ�
void Engine::CreateMultipleRenderTarget()
{
	// ���� ���ٽ� �ؽ�ó ����
	shared_ptr<Texture> dsTexture = Resources::Get().CreateTexture(L"DepthStencil",
		DXGI_FORMAT_D32_FLOAT, width, height,
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	// SwapChain Group
	{
		// 2���� ���� Ÿ���� ���� ���� ����
		vector<RenderTarget> rtVec(2);

		for (UINT32 i = 0; i < 2; ++i)
		{
			// SwapChainTarget �̸� ����
			wstring name = L"SwapChainTarget_" + std::to_wstring(i);

			// SwapChain ���ۿ��� ���ҽ��� ������ �ؽ�ó ����
			ComPtr<ID3D12Resource> resource;
			swapChain->GetSwapChain()->GetBuffer(i, IID_PPV_ARGS(&resource));
			rtVec[i].target = Resources::Get().CreateTextureFromResource(name, resource);
		}

		// SwapChain�� MultipleRenderTarget ���� �� �ʱ�ȭ
		mrts[static_cast<UINT8>(RENDER_TARGET_TYPE::SWAP_CHAIN)] = make_shared<MultipleRenderTarget>();
		mrts[static_cast<UINT8>(RENDER_TARGET_TYPE::SWAP_CHAIN)]->Create(RENDER_TARGET_TYPE::SWAP_CHAIN, rtVec, dsTexture);
	}

	// Deferred Group
	{
		// 3���� ���� Ÿ���� ���� ���� ����
		vector<RenderTarget> rtVec(RENDER_TARGET_BUFFER_MEMBER_COUNT);

		// PositionTarget �ؽ�ó ����
		rtVec[0].target = Resources::Get().CreateTexture(L"PositionTarget",
			DXGI_FORMAT_R32G32B32A32_FLOAT, width, height,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

		// NormalTarget �ؽ�ó ����
		rtVec[1].target = Resources::Get().CreateTexture(L"NormalTarget",
			DXGI_FORMAT_R32G32B32A32_FLOAT, width, height,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

		// DiffuseTarget �ؽ�ó ����
		rtVec[2].target = Resources::Get().CreateTexture(L"DiffuseTarget",
			DXGI_FORMAT_R8G8B8A8_UNORM, width, height,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

		// Buffer�� MultipleRenderTarget ���� �� �ʱ�ȭ
		mrts[static_cast<UINT8>(RENDER_TARGET_TYPE::BUFFER)] = make_shared<MultipleRenderTarget>();
		mrts[static_cast<UINT8>(RENDER_TARGET_TYPE::BUFFER)]->Create(RENDER_TARGET_TYPE::BUFFER, rtVec, dsTexture);
	}
}

void Engine::RenderBegin()
{
	cmdQueue->RenderBegin(&viewPort, &scissorRect);
}

void Engine::RenderEnd()
{
	cmdQueue->RenderEnd();
}

void Engine::ResizeWindow(int _width, int _height)
{
	width = _width;
	height = _height;

	RECT rect = { 0, 0, width, height };

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	SetWindowPos(hwnd, 0, 100, 100, width, height, 0);

	//����
	//depthStencilBuffer->Init(width, height);
}
