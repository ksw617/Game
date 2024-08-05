#include "pch.h"
#include "Resources.h"



shared_ptr<Mesh> Resources::LoadSphereMesh()
{
    shared_ptr<Mesh> findMesh = GetResource<Mesh>(L"Sphere");
    if (findMesh)
        return findMesh;

    float radius = 0.5f; 
    UINT32 stackCount = 20; 
    UINT32 sliceCount = 20; 

    vector<Vertex> vec; 

    Vertex v; 

    v.pos = Vector3(0.0f, radius, 0.0f);
    v.uv = Vector2(0.5f, 0.0f);
    v.normal = v.pos;
    v.normal.Normalize(); 
    v.tangent = Vector3(1.0f, 0.0f, 1.0f);
    vec.push_back(v); 

    float stackAngle = XM_PI / stackCount;
    float sliceAngle = XM_2PI / sliceCount; 

    float deltaU = 1.f / static_cast<float>(sliceCount); 
    float deltaV = 1.f / static_cast<float>(stackCount);

    for (UINT32 y = 1; y <= stackCount - 1; ++y)
    {
        float phi = y * stackAngle;

        for (UINT32 x = 0; x <= sliceCount; ++x)
        {
            float theta = x * sliceAngle; 

            v.pos.x = radius * sinf(phi) * cosf(theta);
            v.pos.y = radius * cosf(phi);
            v.pos.z = radius * sinf(phi) * sinf(theta);

            v.uv = Vector2(deltaU * x, deltaV * y);

            v.normal = v.pos;
            v.normal.Normalize();

            v.tangent.x = -radius * sinf(phi) * sinf(theta);
            v.tangent.y = 0.0f;
            v.tangent.z = radius * sinf(phi) * cosf(theta);
            v.tangent.Normalize();

            vec.push_back(v);
        }
    }

    v.pos = Vector3(0.0f, -radius, 0.0f); 
    v.uv = Vector2(0.5f, 1.0f); 
    v.normal = v.pos;
    v.normal.Normalize();
    v.tangent = Vector3(1.0f, 0.0f, 0.0f);
    vec.push_back(v); 

    vector<UINT32> idx; 

    for (UINT32 i = 0; i <= sliceCount; ++i)
    {
        idx.push_back(0);
        idx.push_back(i + 2); 
        idx.push_back(i + 1); 
    }

  
    UINT32 ringVertexCount = sliceCount + 1;
    for (UINT32 y = 0; y < stackCount - 2; ++y)
    {
        for (UINT32 x = 0; x < sliceCount; ++x)
        {
            idx.push_back(1 + y * ringVertexCount + x);
            idx.push_back(1 + y * ringVertexCount + x + 1); 
            idx.push_back(1 + (y + 1) * ringVertexCount + x); 

            idx.push_back(1 + (y + 1) * ringVertexCount + x); 
            idx.push_back(1 + y * ringVertexCount + x + 1); 
            idx.push_back(1 + (y + 1) * ringVertexCount + x + 1); 
        }
    }

    UINT32 bottomIndex = static_cast<UINT32>(vec.size()) - 1;
    UINT32 lastRingStartIndex = bottomIndex - ringVertexCount;
    for (UINT32 i = 0; i < sliceCount; ++i)
    {
        idx.push_back(bottomIndex); 
        idx.push_back(lastRingStartIndex + i);
        idx.push_back(lastRingStartIndex + i + 1); 
    }

    shared_ptr<Mesh> mesh = make_shared<Mesh>();
    mesh->Init(vec, idx);

    Add(L"Sphere", mesh);

    return mesh;
}


void Resources::Init()
{
    CreateShader();
}

void Resources::CreateShader()
{
    // Skybox 설정
    {
        // 셰이더 설정 정보 구조체 초기화
        ShaderInfo info =
        {
            RASTERIZER_TYPE::CULL_NONE,     // 모든 면을 렌더링 (컬링 없음)
            DEPTH_STENCIL_TYPE::LESS_EQUAL  // 깊이 값이 작거나 같은 경우 통과
        };

        // 셰이더 객체 생성 및 초기화
        shared_ptr<Shader> shader = make_shared<Shader>();
        shader->Init(L"..\\Resources\\Shader\\Skybox.fx", info); // Skybox 셰이더 파일 경로
        Add<Shader>(L"Skybox", shader); // 리소스 매니저에 Skybox 셰이더 추가
    }

    // Default 설정 (기본)
    {
        // 셰이더 설정 정보 구조체 초기화 (기본값 사용)
        ShaderInfo info =
        {
        };

        // 셰이더 객체 생성 및 초기화
        shared_ptr<Shader> shader = make_shared<Shader>();
        shader->Init(L"..\\Resources\\Shader\\Default.fx", info); // Default 셰이더 파일 경로
        Add<Shader>(L"Default", shader); // 리소스 매니저에 Default 셰이더 추가
    }


}

shared_ptr<Mesh> Resources::LoadRectangleMesh()
{
    //"Rectangle" 리소스를 검색하여 이미 존재하는지 확인
    shared_ptr<Mesh> findMesh = GetResource<Mesh>(L"Rectangle");
    if (findMesh)
        return findMesh;    //이미 존재하는 경우, 기존 매쉬를 반환

    float wHalf = 0.5f;    //사각형의 너비의 절반
    float hHalf = 0.5f;    //사각형의 높이의 절반

    vector<Vertex> vec(4);  //4개의 정점을 저장할 벡터 생성

    //앞면 정점 설정
    vec[0] = Vertex(Vector3(-wHalf, -hHalf, 0.f), Vector2(0.0f, 1.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f));
    vec[1] = Vertex(Vector3(-wHalf, +hHalf, 0.f), Vector2(0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f));
    vec[2] = Vertex(Vector3(+wHalf, +hHalf, 0.f), Vector2(1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f));
    vec[3] = Vertex(Vector3(+wHalf, -hHalf, 0.f), Vector2(1.0f, 1.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f));

    vector<UINT32> idx(6);  //6개의 인덱스를 저장할 벡터 생성

    //앞면 인덱스 설정
    idx[0] = 0; idx[1] = 1; idx[2] = 2;
    idx[3] = 0; idx[4] = 2; idx[5] = 3;

    //새로운 메쉬 객체 생성
    shared_ptr<Mesh> mesh = make_shared<Mesh>();
    mesh->Init(vec, idx);   // mesh 초기화
    Add(L"Rectangle", mesh);// 리소스에 "Rectangle"이라는 키로 mesh 추가

    return mesh;    //생성된 메쉬 반환

}

shared_ptr<Mesh> Resources::LoadCubeMesh()
{
	shared_ptr<Mesh> findMesh = GetResource<Mesh>(L"Cube");

	if (findMesh)
		return findMesh;

	float wHalf = 0.5f;
	float hHalf = 0.5f;
	float dHalf = 0.5f;

	vector<Vertex> vec(24);

	//앞면 정점 설정
	vec[0] = Vertex(Vector3(-wHalf, -hHalf, -dHalf), Vector2(0.0f, 1.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f));
	vec[1] = Vertex(Vector3(-wHalf, +hHalf, -dHalf), Vector2(0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f));
	vec[2] = Vertex(Vector3(+wHalf, +hHalf, -dHalf), Vector2(1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f));
	vec[3] = Vertex(Vector3(+wHalf, -hHalf, -dHalf), Vector2(1.0f, 1.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f));

	//뒷면 정점 설정
	vec[4] = Vertex(Vector3(-wHalf, -hHalf, +dHalf), Vector2(1.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f));
	vec[5] = Vertex(Vector3(+wHalf, -hHalf, +dHalf), Vector2(0.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f));
	vec[6] = Vertex(Vector3(+wHalf, +hHalf, +dHalf), Vector2(0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f));
	vec[7] = Vertex(Vector3(-wHalf, +hHalf, +dHalf), Vector2(1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f));

	//윗면 정점 설정
	vec[8]  = Vertex(Vector3(-wHalf, +hHalf, -dHalf), Vector2(0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f));
	vec[9]  = Vertex(Vector3(-wHalf, +hHalf, +dHalf), Vector2(0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f));
	vec[10] = Vertex(Vector3(+wHalf, +hHalf, +dHalf), Vector2(1.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f));
	vec[11] = Vertex(Vector3(+wHalf, +hHalf, -dHalf), Vector2(1.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f));

	//아랫면 정점 설정
	vec[12] = Vertex(Vector3(-wHalf, -hHalf, -dHalf), Vector2(1.0f, 1.0f), Vector3(0.0f, -1.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f));
	vec[13] = Vertex(Vector3(+wHalf, -hHalf, -dHalf), Vector2(0.0f, 1.0f), Vector3(0.0f, -1.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f));
	vec[14] = Vertex(Vector3(+wHalf, -hHalf, +dHalf), Vector2(0.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f));
	vec[15] = Vertex(Vector3(-wHalf, -hHalf, +dHalf), Vector2(1.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f));

	//왼쪽면 정점 설정
	vec[16] = Vertex(Vector3(-wHalf, -hHalf, +dHalf), Vector2(0.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f));
	vec[17] = Vertex(Vector3(-wHalf, +hHalf, +dHalf), Vector2(0.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f));
	vec[18] = Vertex(Vector3(-wHalf, +hHalf, -dHalf), Vector2(1.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f));
	vec[19] = Vertex(Vector3(-wHalf, -hHalf, -dHalf), Vector2(1.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f));

	//오른쪽면 정점 설정
	vec[20] = Vertex(Vector3(+wHalf, -hHalf, -dHalf), Vector2(0.0f, 1.0f), Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f));
	vec[21] = Vertex(Vector3(+wHalf, +hHalf, -dHalf), Vector2(0.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f));
	vec[22] = Vertex(Vector3(+wHalf, +hHalf, +dHalf), Vector2(1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f));
	vec[23] = Vertex(Vector3(+wHalf, -hHalf, +dHalf), Vector2(1.0f, 1.0f), Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f));

	vector<UINT32> idx(36);

	//앞면 인덱스 설정
	idx[0] = 0; idx[1] = 1; idx[2] = 2;
	idx[3] = 0; idx[4] = 2; idx[5] = 3;
	//뒤면 인덱스 설정
	idx[6] = 4; idx[7] = 5; idx[8] = 6;
	idx[9] = 4; idx[10] = 6; idx[11] = 7;
	//윗면 인덱스 설정
	idx[12] = 8; idx[13] = 9; idx[14] = 10;
	idx[15] = 8; idx[16] = 10; idx[17] = 11;
	//아랫면 인덱스 설정
	idx[18] = 12; idx[19] = 13; idx[20] = 14;
	idx[21] = 12; idx[22] = 14; idx[23] = 15;
	//왼쪽면 인덱스 설정
	idx[24] = 16; idx[25] = 17; idx[26] = 18;
	idx[27] = 16; idx[28] = 18; idx[29] = 19;
	//오른쪽면 인덱스 설정
	idx[30] = 20; idx[31] = 21; idx[32] = 22;
	idx[33] = 20; idx[34] = 22; idx[35] = 23;


	shared_ptr<Mesh> mesh = make_shared<Mesh>();
	mesh->Init(vec, idx);


	Add(L"Cube", mesh);

	return mesh;
}