#include "pch.h"
#include "Resources.h"



shared_ptr<Mesh> Resources::LoadSphereMesh()
{
    // 이미 "Sphere"라는 키로 로드된 메쉬가 있는지 확인
    shared_ptr<Mesh> findMesh = GetResource<Mesh>(L"Sphere");
    if (findMesh)
        return findMesh; // 이미 존재하면 해당 메쉬를 반환

    float radius = 0.5f; // 구의 반지름 설정
    UINT32 stackCount = 20; // 가로 방향으로의 분할 수 설정
    UINT32 sliceCount = 20; // 세로 방향으로의 분할 수 설정

    vector<Vertex> vec; // 정점을 저장할 벡터 생성

    Vertex v; // 정점 구조체 인스턴스

    // 북극 정점 설정
    v.pos = Vector3(0.0f, radius, 0.0f); // 북극 정점의 위치
    v.uv = Vector2(0.5f, 0.0f); // 북극 정점의 텍스처 좌표
    v.normal = v.pos; // 법선 벡터 설정
    v.normal.Normalize(); // 법선 벡터 정규화
    v.tangent = Vector3(1.0f, 0.0f, 1.0f); // 접선 벡터 설정
    vec.push_back(v); // 벡터에 북극 정점 추가

    float stackAngle = XM_PI / stackCount; // 스택 간의 각도
    float sliceAngle = XM_2PI / sliceCount; // 슬라이스 간의 각도

    float deltaU = 1.f / static_cast<float>(sliceCount); // U 좌표의 변화량
    float deltaV = 1.f / static_cast<float>(stackCount); // V 좌표의 변화량

    // 각 스택마다 정점을 계산 (북극과 남극을 제외한 부분)
    for (UINT32 y = 1; y <= stackCount - 1; ++y)
    {
        float phi = y * stackAngle; // 현재 스택의 각도

        // 각 슬라이스마다 정점을 계산
        for (UINT32 x = 0; x <= sliceCount; ++x)
        {
            float theta = x * sliceAngle; // 현재 슬라이스의 각도

            // 정점의 위치 계산
            v.pos.x = radius * sinf(phi) * cosf(theta);
            v.pos.y = radius * cosf(phi);
            v.pos.z = radius * sinf(phi) * sinf(theta);

            // 텍스처 좌표 계산
            v.uv = Vector2(deltaU * x, deltaV * y);

            // 법선 벡터 계산 및 정규화
            v.normal = v.pos;
            v.normal.Normalize();

            // 접선 벡터 계산 및 정규화
            v.tangent.x = -radius * sinf(phi) * sinf(theta);
            v.tangent.y = 0.0f;
            v.tangent.z = radius * sinf(phi) * cosf(theta);
            v.tangent.Normalize();

            // 정점을 벡터에 추가
            vec.push_back(v);
        }
    }

    // 남극 정점 설정
    v.pos = Vector3(0.0f, -radius, 0.0f); // 남극 정점의 위치
    v.uv = Vector2(0.5f, 1.0f); // 남극 정점의 텍스처 좌표
    v.normal = v.pos; // 법선 벡터 설정
    v.normal.Normalize(); // 법선 벡터 정규화
    v.tangent = Vector3(1.0f, 0.0f, 0.0f); // 접선 벡터 설정
    vec.push_back(v); // 벡터에 남극 정점 추가

    vector<UINT32> idx; // 인덱스를 저장할 벡터 생성

    // 북극 인덱스 설정
    for (UINT32 i = 0; i <= sliceCount; ++i)
    {
        // 북극에서 각 슬라이스로 연결되는 삼각형을 생성
        idx.push_back(0); // 북극 정점 인덱스
        idx.push_back(i + 2); // 현재 슬라이스의 다음 정점 인덱스
        idx.push_back(i + 1); // 현재 슬라이스의 정점 인덱스
    }

    // 몸통 인덱스 설정
    UINT32 ringVertexCount = sliceCount + 1; // 각 고리에 있는 정점의 수
    for (UINT32 y = 0; y < stackCount - 2; ++y)
    {
        for (UINT32 x = 0; x < sliceCount; ++x)
        {
            // 현재 고리와 다음 고리를 연결하는 두 개의 삼각형을 생성
            idx.push_back(1 + y * ringVertexCount + x); // 현재 고리의 현재 슬라이스 정점
            idx.push_back(1 + y * ringVertexCount + x + 1); // 현재 고리의 다음 슬라이스 정점
            idx.push_back(1 + (y + 1) * ringVertexCount + x); // 다음 고리의 현재 슬라이스 정점

            idx.push_back(1 + (y + 1) * ringVertexCount + x); // 다음 고리의 현재 슬라이스 정점
            idx.push_back(1 + y * ringVertexCount + x + 1); // 현재 고리의 다음 슬라이스 정점
            idx.push_back(1 + (y + 1) * ringVertexCount + x + 1); // 다음 고리의 다음 슬라이스 정점
        }
    }

    // 남극 인덱스 설정
    UINT32 bottomIndex = static_cast<UINT32>(vec.size()) - 1; // 남극 정점의 인덱스
    UINT32 lastRingStartIndex = bottomIndex - ringVertexCount; // 마지막 고리의 시작 정점 인덱스
    for (UINT32 i = 0; i < sliceCount; ++i)
    {
        // 남극에서 각 슬라이스로 연결되는 삼각형을 생성
        idx.push_back(bottomIndex); // 남극 정점 인덱스
        idx.push_back(lastRingStartIndex + i); // 마지막 고리의 현재 슬라이스 정점
        idx.push_back(lastRingStartIndex + i + 1); // 마지막 고리의 다음 슬라이스 정점
    }

    // 새로운 메쉬 객체 생성 및 초기화
    shared_ptr<Mesh> mesh = make_shared<Mesh>();
    mesh->Init(vec, idx);

    // "Sphere"라는 키로 메쉬를 리소스에 추가
    Add(L"Sphere", mesh);

    return mesh; // 메쉬 반환
}


shared_ptr<Mesh> Resources::LoadCubeMesh()
{
	//이미 "Cube"라는 키로 로드된 매쉬가 있는지 확인
	shared_ptr<Mesh> findMesh = GetResource<Mesh>(L"Cube");

	//이미 있으면 반환
	if (findMesh)
		return findMesh;

	//큐브의 절반 크기 설정
	float wHalf = 0.5f;
	float hHalf = 0.5f;
	float dHalf = 0.5f;

	//정점 배열 생성(24개의 정점) //각 꼭지점
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

	//인덱스 배열 생성 (36개의 인덱스)
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

	//새로운 매쉬 객체 생성 및 초기화
	shared_ptr<Mesh> mesh = make_shared<Mesh>();
	mesh->Init(vec, idx);

	//"Cube"라는 키로 매쉬를 리소스에 추가
	Add(L"Cube", mesh);

	return mesh;
}