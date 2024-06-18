//상수 버퍼 TEST_B0, 레지스터 b0에 바인딩
cbuffer TEST_B0 : register(b0)
{
    float4 offset0; //위치 오프셋 값
}

//상수 버퍼 TEST_B1, 레지스터 b1에 바인딩
cbuffer TEST_B1 : register(b1)
{
    float4 offset1; //색상 오프셋 값
}



struct VS_IN
{
    float3 pos : POSITION;
    float4 color : COLOR; 
};

struct VS_OUT
{
    float4 pos : SV_Position;
    float4 color : COLOR;     
};

VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0; 
    output.pos = float4(input.pos, 1.f);
    
    //위치 오프셋 추가
    output.pos += offset0;
    
    output.color = input.color;
    
    //색상 오프셋 추가
    output.color += offset1;
    
    return output;

}

float4 PS_Main(VS_OUT input) : SV_Target
{
    return input.color;
}