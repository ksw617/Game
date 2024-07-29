#ifndef _DEFAULT_FX_
#define _DEFAULT_FX_

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

LightColor CalculateLightColor(int lightIndex, float3 viewNormal, float3 viewPos)
{
    LightColor color = (LightColor) 0.f;
    
    float3 viewLightDir = (float3) 0.f;
    float diffuseRatio = 0.f; 
    float specularRatio = 0.f; 
    float distanceRatio = 1.f; 
    
    if (light[lightIndex].lightType == 0)
    {
        viewLightDir = normalize(mul(float4(light[lightIndex].direction.xyz, 0.f), matView).xyz);
        diffuseRatio = saturate(dot(-viewLightDir, viewNormal));
    }
    
    else if (light[lightIndex].lightType == 1)
    {
        float3 viewLightPos = mul(float4(light[lightIndex].position.xyz, 1.f), matView).xyz;
        viewLightDir = normalize(viewPos - viewLightPos);
        diffuseRatio = saturate(dot(-viewLightDir, viewNormal));
        
        float dist = distance(viewPos, viewLightPos);
        distanceRatio = light[lightIndex].range == 0.f ? 0.f : saturate(1.f - pow(dist / light[lightIndex].range, 2));

    }
    else
    {
        float3 viewLightPos = mul(float4(light[lightIndex].position.xyz, 1.f), matView).xyz;
        viewLightDir = normalize(viewPos - viewLightPos);
        
        diffuseRatio = saturate(dot(-viewLightDir, viewNormal));
        
        if (light[lightIndex].range == 0.f)
        {
            distanceRatio = 0.f;
        }
        else
        {
            float halfAngle = light[lightIndex].angle / 2;
            float3 viewLightVec = viewPos - viewLightPos;
            float3 viewCenterLightDir = normalize(mul(float4(light[lightIndex].direction.xyz, 0.f), matView).xyz);
            
            float centerDist = dot(viewLightVec, viewCenterLightDir);
            
            distanceRatio = saturate(1.f - centerDist / light[lightIndex].range);
            float lightAngle = acos(dot(normalize(viewLightVec), viewCenterLightDir));
            if (centerDist < 0.f || centerDist > light[lightIndex].range || lightAngle > halfAngle)
                distanceRatio = 0.f;
            else
                distanceRatio = saturate(1.f - pow(centerDist / light[lightIndex].range, 2));

        }
      
    }

    float3 reflectionDir = normalize(viewLightDir + 2 * (saturate(dot(-viewLightDir, viewNormal)) * viewNormal));
    float3 eyeDir = normalize(viewPos);
    specularRatio = pow(saturate(dot(-eyeDir, reflectionDir)), 5);

    color.diffuse = light[lightIndex].color.diffuse * diffuseRatio * distanceRatio;
    color.ambient = light[lightIndex].color.ambient * distanceRatio;
    color.specular = light[lightIndex].color.specular * specularRatio * distanceRatio;
   
    return color;

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