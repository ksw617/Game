#pragma once
#include "Device.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "RootSignature.h"
#include "Mesh.h"
#include "Shader.h"
#include "ConstantBuffer.h"
#include "TableDescriptor.h"

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
	shared_ptr<ConstantBuffer> constBuffer;
	//TableDescriptor 포인터
	shared_ptr<TableDescriptor> tableDesc;
public:
	shared_ptr<Device> GetDevice() { return device; }
	shared_ptr<CommandQueue> GetCmdQueue() { return cmdQueue; }
	shared_ptr<SwapChain> GetSwapChain() { return swapChain; }
	shared_ptr<RootSignature> GetRootSignature() { return rootSignature; }
	shared_ptr<ConstantBuffer> GetConstBuffer() { return constBuffer; }
	//Get함수
	shared_ptr<TableDescriptor> GetTableDesc() { return tableDesc; }

public:
	void Init(HWND _hwnd, int _width, int _height, bool _windowed);
	void Render();

public:
	void RenderBegin();
	void RenderEnd();

	void ResizeWindow(int _width, int _height);
};

