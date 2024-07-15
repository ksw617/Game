#pragma once

#define _HAS_STD_BYTE 0

#include <Windows.h>
#include <memory>
#include <string>
#include <vector>
#include <array>
#include <list>
#include <map>

using namespace std;

#include <filesystem>

#include "d3dx12.h"
#include <d3d12.h>

#include "SimpleMath.h" 

#include <wrl.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>

#include <DirectXTex/DirectXTex.h>
#include <DirectXTex/DirectXTex.inl>

using namespace DirectX;
using namespace DirectX::PackedVector;
using namespace DirectX::SimpleMath; 
using namespace Microsoft::WRL;

#pragma comment(lib, "d3d12")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "dxguid")
#pragma comment(lib, "d3dcompiler")

#ifdef _DEBUG   // _DEBUG
#pragma comment(lib, "Debug\\DirectXTex.lib")
#else			// _RELEASE
#pragma comment(lib, "Release\\DirectXTex.lib")
#endif


//Matrix·Î º¯°æ
struct TransformMatrix
{
	Matrix offset;

};

struct Vertex
{
	Vector3 pos;
	Vector4 color;
	Vector2 uv;
};

enum class CBV_REGISTER : UINT8	 
{
	b0,	 
	b1,  
	b2,	 
	b3,	 
	b4,	 

	END

};

enum class SRV_REGISTER : UINT8	
{
	t0 = static_cast<UINT8>(CBV_REGISTER::END),
	t1,  
	t2,
	t3,	
	t4,	

	END 
};

enum 
{
	 CBV_REGISTER_COUNT = CBV_REGISTER::END,

	 SRV_REGISTER_COUNT = static_cast<UINT8>(SRV_REGISTER::END) - CBV_REGISTER_COUNT,
	 REGISTER_COUNT = CBV_REGISTER_COUNT + SRV_REGISTER_COUNT, 
};