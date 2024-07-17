#pragma once
#include "Singleton.h"  // Singleton 호출
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

class Engine : public Singleton<Engine>
{
	//Singleton 클라스가 private 생성자에 접근할수 있게
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
	shared_ptr<DepthStencilBuffer> depthStencilBuffer;	

	shared_ptr<Input> input; 
	shared_ptr<Timer> timer; 

	vector<shared_ptr<ConstantBuffer>> constantBuffers;
public:
	shared_ptr<Device> GetDevice() { return device; }
	shared_ptr<CommandQueue> GetCmdQueue() { return cmdQueue; }
	shared_ptr<SwapChain> GetSwapChain() { return swapChain; }
	shared_ptr<RootSignature> GetRootSignature() { return rootSignature; }
	shared_ptr<TableDescriptor> GetTableDesc() { return tableDesc; }
	shared_ptr<DepthStencilBuffer> GetDepthStencilBuffer() { return depthStencilBuffer; }

	shared_ptr<Input> GetInput() { return input; }
	shared_ptr<Timer> GetTimer() { return timer; }

	shared_ptr<ConstantBuffer> GetConstantBuffer(CONSTANT_BUFFER_TYPE type) { return constantBuffers[static_cast<UINT8>(type)]; }

	int GetWidth() const { return width; }
	int GetHeight() const { return height; }
public:
	void Init(HWND _hwnd, int _width, int _height, bool _windowed);
	void Render();
public:
	void Update(); 
	void ShowFPS(); 

	void CreateConstantBuffer(CBV_REGISTER reg, UINT32 bufferSize, UINT32 count);
public:
	void RenderBegin();
	void RenderEnd();

	void ResizeWindow(int _width, int _height);
};

