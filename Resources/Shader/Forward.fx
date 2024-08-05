#ifndef _FORWARD_FX_
#define _FORWARD_FX_

#include "Params.fx"
#include "Funcs.fx"


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

float4 PS_Main(VS_OUT input) : SV_Target
{
    float4 color = float4(1.f, 1.f, 1.f, 1.f);
    
    if (tex_on_0)
            color = tex_0.Sample(sam_0, input.uv);

    float3 viewNormal = input.viewNormal;
    
    if(tex_on_1)
    {
        float3 tangentSpaceNormal = tex_1.Sample(sam_0, input.uv).xyz;

        tangentSpaceNormal = (tangentSpaceNormal - 0.5f) * 2.f;
        float3x3 matTBN = { input.viewTangent, input.viewBinormal, input.viewNormal };
        viewNormal = normalize(mul(tangentSpaceNormal, matTBN));

    }
    
    LightColor totalColor = (LightColor) 0.f;
    
    for (int i = 0; i < lightCount; i++)
    {
        LightColor color = CalculateLightColor(i, viewNormal, input.viewPos);
        totalColor.diffuse += color.diffuse;
        totalColor.ambient += color.ambient;
        totalColor.specular += color.specular;

    }
    
    color.xyz = (totalColor.diffuse.xyz * color.xyz)
    + totalColor.ambient.xyz * color.xyz
    + totalColor.specular.xyz;
    
    return color;
}

#endif