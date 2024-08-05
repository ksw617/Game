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

// Pixel Shader ��� ����ü
struct PS_OUT
{
    float4 position : SV_Target0; // �� ���������� ��ġ
    float4 normal : SV_Target1; // �� ���������� ���
    float4 color : SV_Target2; // ����
};


PS_OUT PS_Main(VS_OUT input)
{
    PS_OUT output = (PS_OUT) 0; // PS_OUT ����ü �ʱ�ȭ

    float4 color = float4(1.f, 1.f, 1.f, 1.f); // �⺻ ���� ���� (���)
    if (tex_on_0) // ù ��° �ؽ�ó�� Ȱ��ȭ�Ǿ� �ִ� ���
        color = tex_0.Sample(sam_0, input.uv); // �ؽ�ó ���� ���ø�

    float3 viewNormal = input.viewNormal; // �� ���������� ��ְ��ʱ�ȭ
    if (tex_on_1) // �� ��° �ؽ�ó�� Ȱ��ȭ�Ǿ� �ִ� ��� (��� ��)
    {

        // [0,255] �������� [0,1]�� ��ȯ
        float3 tangentSpaceNormal = tex_1.Sample(sam_0, input.uv).xyz;
        // [0,1] �������� [-1,1]�� ��ȯ
        tangentSpaceNormal = (tangentSpaceNormal - 0.5f) * 2.f;
        // TBN ����� �̿��� ��ֺ��͸� ��ȯ
        float3x3 matTBN = { input.viewTangent, input.viewBinormal, input.viewNormal };
        viewNormal = normalize(mul(tangentSpaceNormal, matTBN));


    }
    
   // ��� ����ü�� ���� ����
    output.position = float4(input.viewPos.xyz, 0.f); // �� ���������� ��ġ ����
    output.normal = float4(viewNormal.xyz, 0.f); // �� ���������� ��ְ� ����
    output.color = color; // ���� ����

    return output; // ��� ����ü ��ȯ
}

#endif