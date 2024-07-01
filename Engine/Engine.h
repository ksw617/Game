#pragma once
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
#include "Timer.h" // �߰�

class Engine
{
private:   
	Engine() = default; 
	~Engine() = default;
public:
	static Engine& Get()
	{
		static Engine instnace;
		return instnace;
	}

public:
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;

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
	//shared_ptr<ConstantBuffer> constBuffer; //����
	shared_ptr<TableDescriptor> tableDesc;
	shared_ptr<DepthStencilBuffer> depthStencilBuffer;	

	shared_ptr<Input> input; 
	shared_ptr<Timer> timer; 

	//constantBuffer ����Ʈ �����͵��� ���� vector
	vector<shared_ptr<ConstantBuffer>> constantBuffers;
public:
	shared_ptr<Device> GetDevice() { return device; }
	shared_ptr<CommandQueue> GetCmdQueue() { return cmdQueue; }
	shared_ptr<SwapChain> GetSwapChain() { return swapChain; }
	shared_ptr<RootSignature> GetRootSignature() { return rootSignature; }
	//shared_ptr<ConstantBuffer> GetConstBuffer() { return constBuffer; }   //����
	shared_ptr<TableDescriptor> GetTableDesc() { return tableDesc; }
	shared_ptr<DepthStencilBuffer> GetDepthStencilBuffer() { return depthStencilBuffer; }

	shared_ptr<Input> GetInput() { return input; }
	shared_ptr<Timer> GetTimer() { return timer; }

	//CONSTANT_BUFFER_TYPE�� ���� ���ϴ� ConstantBuffer ��ȯ�ϴ� Get �Լ�
	shared_ptr<ConstantBuffer> GetConstantBuffer(CONSTANT_BUFFER_TYPE type) { return constantBuffers[static_cast<UINT8>(type)]; }

public:
	void Init(HWND _hwnd, int _width, int _height, bool _windowed);
	void Render();
public:
	void Update(); 
	void ShowFPS(); 

	//ConstantBuffer ����� �Լ�
	void CreateConstantBuffer(CBV_REGISTER reg, UINT32 bufferSize, UINT32 count);
public:
	void RenderBegin();
	void RenderEnd();

	void ResizeWindow(int _width, int _height);
};

