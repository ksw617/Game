#ifndef _DEFERRED_FX_
#define _DEFERRED_FX_

#include "Params.fx"


struct VS_IN
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL; 
    float3 tangent : TANGENT;
};

struct VS_OUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
    float3 viewPos : POSITION;
    float3 viewNormal : NORMAL;
    float3 viewTangent : TANGENT;
    float3 viewBinormal : BINORMAL;
};

VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;
    
    output.pos = mul(float4(input.pos, 1.f), matWVP);
    output.uv = input.uv;
    
    output.viewPos = mul(float4(input.pos, 1.f), matWV).xyz;
    output.viewNormal = normalize(mul(float4(input.normal, 0.f), matWV).xyz);
    output.viewTangent = normalize(mul(float4(input.tangent, 0.f), matWV).xyz);
    output.viewBinormal = normalize(cross(output.viewTangent, output.viewNormal));
    
    return output;

}

// Pixel Shader 출력 구조체
struct PS_OUT
{
    float4 position : SV_Target0; // 뷰 공간에서의 위치
    float4 normal : SV_Target1; // 뷰 공간에서의 노멀
    float4 color : SV_Target2; // 색상
};


PS_OUT PS_Main(VS_OUT input)
{
    PS_OUT output = (PS_OUT) 0; // PS_OUT 구조체 초기화

    float4 color = float4(1.f, 1.f, 1.f, 1.f); // 기본 색상 설정 (흰색)
    if (tex_on_0) // 첫 번째 텍스처가 활성화되어 있는 경우
        color = tex_0.Sample(sam_0, input.uv); // 텍스처 색상 샘플링

    float3 viewNormal = input.viewNormal; // 뷰 공간에서의 노멀값초기화
    if (tex_on_1) // 두 번째 텍스처가 활성화되어 있는 경우 (노멀 맵)
    {

        // [0,255] 범위에서 [0,1]로 변환
        float3 tangentSpaceNormal = tex_1.Sample(sam_0, input.uv).xyz;
        // [0,1] 범위에서 [-1,1]로 변환
        tangentSpaceNormal = (tangentSpaceNormal - 0.5f) * 2.f;
        // TBN 행렬을 이용해 노멀벡터를 변환
        float3x3 matTBN = { input.viewTangent, input.viewBinormal, input.viewNormal };
        viewNormal = normalize(mul(tangentSpaceNormal, matTBN));


    }
    
   // 출력 구조체에 값을 설정
    output.position = float4(input.viewPos.xyz, 0.f); // 뷰 공간에서의 위치 설정
    output.normal = float4(viewNormal.xyz, 0.f); // 뷰 공간에서의 노멀값 설정
    output.color = color; // 색상 설정

    return output; // 출력 구조체 반환
}

#endif