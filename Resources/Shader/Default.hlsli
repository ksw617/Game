//Vertex Shader In
struct VS_IN
{
    float3 pos : POSITION; // 정점 위치
    float4 color : COLOR;  // 정점 색상
};

//Vetex Shader Out
struct VS_OUT
{
    float4 pos : SV_Position; // 변환된 정점 위치 (클립 공간)
    float4 color : COLOR;     // 정점 색상
};

//Vertex Shader Main
VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0; // 출력 구조체 초기화
    //입력 정점 위치를 float4로 변환(w=1.0)
    output.pos = float4(input.pos, 1.f);
    //입력 정점 색상을 그대로 전달
    output.color = input.color;
    
    return output; //출력 구조체 반환

}

//Pixel Shader Main
float4 PS_Main(VS_OUT input) : SV_Target
{
     //입력 정점 색상을 그대로 반환
    return input.color;
}