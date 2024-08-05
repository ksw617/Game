#pragma once
#include "Singleton.h"  
#include "Device.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "RootSignature.h"
#include "Mesh.h"
#include "Shader.h"
#include "ConstantBuffer.h"
#include "TableDescriptor.h"
#include "Texture.h"  
#include "DepthStencilBuffer.h"	

#include "Input.h" 
#include "Timer.h"

#include "MultipleRenderTarget.h" // 추가

class Engine : public Singleton<Engine>
{
	friend class Singleton<Engine>;

private:   
	Engine() = default; 
	~Engine() = default;

private:
	HWND hwnd; 
	int width; 
	int height;
	bool windowed; 

	D3D12_VIEWPORT viewPort = {}; 
	D3D12_RECT scissorRect = {};

	shared_ptr<Device> device;
	shared_ptr<CommandQueue> cmdQueue; 
	shared_ptr<SwapChain> swapChain; 
	shared_ptr<RootSignature> rootSignature;
	shared_ptr<TableDescriptor> tableDesc;
	//shared_ptr<DepthStencilBuffer> depthStencilBuffer;	

	//MultipleRenderTarget 갯수만큼 배열
	array<shared_ptr<MultipleRenderTarget>, RENDER_TARGET_COUNT> mrts;

	shared_ptr<Input> input; 
	shared_ptr<Timer> timer; 

	vector<shared_ptr<ConstantBuffer>> constantBuffers;
public:
	shared_ptr<Device> GetDevice() { return device; }
	shared_ptr<CommandQueue> GetCmdQueue() { return cmdQueue; }
	shared_ptr<SwapChain> GetSwapChain() { return swapChain; }
	shared_ptr<RootSignature> GetRootSignature() { return rootSignature; }
	shared_ptr<TableDescriptor> GetTableDesc() { return tableDesc; }
	//shared_ptr<DepthStencilBuffer> GetDepthStencilBuffer() { return depthStencilBuffer; }

	shared_ptr<Input> GetInput() { return input; }
	shared_ptr<Timer> GetTimer() { return timer; }

	shared_ptr<ConstantBuffer> GetConstantBuffer(CONSTANT_BUFFER_TYPE type) { return constantBuffers[static_cast<UINT8>(type)]; }

	//MultipleRenderTarget Get 함수
	shared_ptr<MultipleRenderTarget> GetMRT(RENDER_TARGET_TYPE type) { return mrts[static_cast<UINT8>(type)]; }

	int GetWidth() const { return width; }
	int GetHeight() const { return height; }
public:
	void Init(HWND _hwnd, int _width, int _height, bool _windowed);
	void Render();
public:
	void Update(); 
	void ShowFPS(); 

	void CreateConstantBuffer(CBV_REGISTER reg, UINT32 bufferSize, UINT32 count);

	//MultipleRenderTarget 만들어 주는 함수
	void CreateMultipleRenderTarget();
public:
	void RenderBegin();
	void RenderEnd();

	void ResizeWindow(int _width, int _height);
};

