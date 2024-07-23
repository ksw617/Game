//조명 색상 정보 저장
struct LightColor
{
    float4 diffuse; // 확산광 색상
    float4 ambient; // 주변광 색상
    float4 specular; // 반사광 색상
};

//조명 정보를 저장
struct LightInfo
{
    LightColor color; //조명 색상 정보
    float4 position; //조명 위치
    float4 direction; //조명 방향
    int lightType; //(0:Directional, 1:Point, 2:Spot)
    float range; //조명 범위(Point, Spot에 해당)
    float angle; //조명 각도(Spot에 해당)
    int padding; //패딩(메모리 정렬을 위해)
};

//전역 파라미터 버퍼: b0 레지스터에 바인딩
cbuffer GLOBAL_PARAMS : register(b0)
{
    int lightCount; //조명갯수
    float3 padding; //패딩(메모리 정렬을 위해)
    LightInfo light[50]; //최대 50개의 조명 정보    
}

//Transform 파라미터는 b1에 바인딩
cbuffer TRANSFORM_PARAMS : register(b1)
{
    row_major matrix matWorld; //월드 행렬
    row_major matrix matView; //뷰 행렬
    row_major matrix matProjection; //투영 행렬 
    row_major matrix matWV; //월드-뷰 행렬
    row_major matrix matWVP; //월드-뷰-프로젝션 행렬

}

//Material 파라미터는 b2에 바인딩
cbuffer MATERIAL_PARAMS : register(b2)
{
    int int_0;
    int int_1;
    int int_2;
    int int_3;
    int int_4;
    
    float float_0;
    float float_1;
    float float_2;
    float float_3;
    float float_4;
}

Texture2D tex_0 : register(t0);
Texture2D tex_1 : register(t1);
Texture2D tex_2 : register(t2);
Texture2D tex_3 : register(t3);
Texture2D tex_4 : register(t4);

SamplerState sam_0 : register(s0);

struct VS_IN
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL; //Normal 벡터
};

struct VS_OUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
    float3 viewPos : POSITION; //뷰 공간 정점 위치
    float3 viewNormal : NORMAL; // 뷰 공간 Normal 벡터
};

VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;
    
    //정점 위치를 변환하여 클립 공간 좌표를 계산
    output.pos = mul(float4(input.pos, 1.f), matWVP);
    output.uv = input.uv;
    
    //뷰 공간 정점 위치와 normal 벡터를 계산
    output.viewPos = mul(float4(input.pos, 1.f), matWV).xyz;
    output.viewNormal = normalize(mul(float4(input.normal, 0.f), matWV).xyz);
    
    return output;

}

//조명 색상 계산 함수
LightColor CalculateLightColor(int lightIndex, float3 viewNormal, float3 viewPos)
{
    LightColor color = (LightColor) 0.f; // 초기 조명 색상 초기화
    
    float3 viewLightDir = (float3) 0.f; //뷰 공간에서의 조명 방향 초기화
    float diffuseRatio = 0.f; //확산광 비율 초기화
    float specularRatio = 0.f; //반사광 비율 초기화
    float distanceRatio = 0.f; //거리 비율 초기화
    
    //Directional Light(다이렉트 조명)
    if (light[lightIndex].lightType == 0)
    {
        //조명 방향을 뷰 공간으로 변환
        viewLightDir = normalize(mul(float4(light[lightIndex].direction.xyz, 0f), matView).xyz);
        //확산광 비율 계산(normal 벡터와 조명 방향의 벡터의 내적)
        diffuseRatio = saturate(dot(-viewLightDir, viewNormal));
    }
    
    //Point Light(포인터 조명)
    else if (light[lightIndex].lightType == 1)
    {
        //조명 위치를 뷰 공간으로 변환
        float3 viewLightPos = mul(float4(light[lightIndex].position.xyz, 1f), matView).xyz;
        //조명 방향 벡터 계산(정점 위치와 조명 위치의 차이)
        viewLightDir = normalize(viewPos - viewLightPos);
        //확산광 비율 계산(normal 벡터와 조명 방향의 벡터의 내적)
        diffuseRatio = saturate(dot(-viewLightDir, viewNormal));
        
        //정점 위치와 조명 위치간의 거리 계산
        float dist = distance(viewPos, viewLightPos);
        //거리 비율 계산(조명 범위에 따른 감쇠)
        distanceRatio = light[lightIndex].range == 0.f ? 0.f : saturate(1.f - pow(dist / light[lightIndex].range, 2));

    }
    //Spot Light(스포트라이트)
    else
    {
        //조명 위치를 뷰 공간으로 변환
        float3 viewLightPos = mul(float4(light[lightIndex].position.xyz, 1f), matView).xyz;

        //조명 방향 벡터 계산(정점 위치와 조명 위치의 차이)
        viewLightDir = normalize(viewPos - viewLightPos);
        
         //확산광 비율 계산(normal 벡터와 조명 방향의 벡터의 내적)
        diffuseRatio = saturate(dot(-viewLightDir, viewNormal));
        
        if (light[lightIndex].range == 0.f)
        {
            distanceRatio = 0.f;
        }
        else
        {
            //스포트라이트의 절반 각도 계산
            float halfAngle = light[lightIndex].angle / 2;
            //뷰 공간에서의 조명 벡터 계산
            float3 viewLightVec = viewPos - viewLightPos;
            //뷰 공간에서의 중심 조명 방향 벡터 계산
            float3 viewCenterLightDir = normalize(mul(float4(light[lightIndex].direction.xyz, 0.f), matView).xyz);
            
            //중심 거리 계산(정점 위치와 조명 방향 벡터의 내적)
            float centerDist = dot(viewLightVec, viewCenterLightDir);
            
            //거리 비율 계산(조명 범위에 따른 감쇠)
            distanceRatio = saturate(1.f - centerDist / light[lightIndex].range);
            //조명 각도 계산(정점 위치와 중심 조명 방향 벡터간의 각도)
            float lightAngle = acos(dot(normalize(viewLightVec), viewCenterLightDir));
            if (centerDist < 0.f || centerDist > light[lightIndex].range || lightAngle > halfAngle)
                distanceRatio = 0.f;
            else
                distanceRatio = saturate(1.f - pow(centerDist / light[lightIndex].range, 2));

        }
      
    }
    
    
      
    //스펙큘러 반사 방향 벡터 계산
    float3 reflectionDir = normalize(viewLightDir + 2 * (saturate(dot(-viewLightDir, viewNormal)) * viewNormal));
    //카메라 방향 벡터 계산
    float3 eyeDir = normalize(viewPos);
    //스펙큘러 반사 비율 계산
    specularRatio = pow(saturate(dot(-eyeDir, reflectionDir)), 2);

    //조명 색상 계산
    color.diffuse = light[lightIndex].color.diffuse * diffuseRatio * distanceRatio;
    color.ambient = light[lightIndex].color.ambient * distanceRatio;
    color.specular = light[lightIndex].color.specular * specularRatio * distanceRatio;
   
    return color;

}

//픽셀 쉐이더 메인 함수
float4 PS_Main(VS_OUT input) : SV_Target
{
    //float4 color = tex_0.Sample(sam_0, input.uv);
    
    //기본 색상 설정(흰색)
    float4 color = float4(1.f, 1.f, 1.f, 1.f);
    
    //전체 조명 색상 초기화
    LightColor totalColor = (LightColor) 0.f;
    
    //각 조명에 대한 색상 계산
    for (int i = 0; i < lightCount; i++)
    {
        LightColor color = CalculateLightColor(i, input.viewNormal, input.viewPos);
        totalColor.diffuse += color.diffuse;
        totalColor.ambient += color.ambient;
        totalColor.specular += color.specular;

    }
    
    //계산된 조명 색상을 적용하여 최종 색상 결정
    color.xyz = (totalColor.diffuse.xyz * color.xyz)
    + totalColor.ambient.xyz * color.xyz
    + totalColor.specular.xyz;
    
    return color;
}