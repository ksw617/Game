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


struct TransformParams
{
	Matrix matWorld;		//월드 행렬 : 모델 좌표계를 월드 좌표계로 변환하는 행렬
	Matrix matView;			//뷰 행렬 : 월드 좌표계를 카메라 좌표계로 변환하는 행렬
	Matrix matProjection;	//투영 행렬 : 카메라 좌표계를 클립 공간으로 변환하는 행렬
	Matrix matWV;			//월드-뷰 행렬 : 모델 좌표계를 카메라 좌표계로 변환하는 행렬(월드 행렬과 뷰 행렬의 곱)
	Matrix matWVP;			//월드-뷰-프로젝션 행렬 : 모델 좌표계를 클립 공간으로 변환하는 행렬(월드-뷰 행렬과 투영 행렬의 곱)

};

struct Vertex
{
	Vertex() {}
	Vertex(Vector3 p, Vector2 u, Vector3 n, Vector3 t)
		: pos(p), uv(u), normal(n), tangent(t) {}

	Vector3 pos;
	Vector2 uv;
	Vector3 normal;
	Vector3 tangent;
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