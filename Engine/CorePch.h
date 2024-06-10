#pragma once
#include <Windows.h>

using namespace std;

//DirectX Header
#include <wrl.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>

//DirectX namespace
using namespace DirectX;
using namespace DirectX::PackedVector;
using namespace Microsoft::WRL;

//DirectX library
#pragma comment(lib, "dxgi")
#pragma comment(lib, "dxguid")
#pragma comment(lib, "d3dcompiler")

//Hello world
void HelloWorld();