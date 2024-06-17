#pragma once
#include <Windows.h>
//추가
#include <memory>
#include <string>
#include <vector>
#include <array>
#include <list>
#include <map>

using namespace std;


#include "d3dx12.h"
#include <d3d12.h>

#include <wrl.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>

using namespace DirectX;
using namespace DirectX::PackedVector;
using namespace Microsoft::WRL;

#pragma comment(lib, "d3d12")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "dxguid")
#pragma comment(lib, "d3dcompiler")

//Vertex 구조체
struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT4 color;
};