#include "pch.h"
#include "Resources.h"



shared_ptr<Mesh> Resources::LoadSphereMesh()
{
    // �̹� "Sphere"��� Ű�� �ε�� �޽��� �ִ��� Ȯ��
    shared_ptr<Mesh> findMesh = GetResource<Mesh>(L"Sphere");
    if (findMesh)
        return findMesh; // �̹� �����ϸ� �ش� �޽��� ��ȯ

    float radius = 0.5f; // ���� ������ ����
    UINT32 stackCount = 20; // ���� ���������� ���� �� ����
    UINT32 sliceCount = 20; // ���� ���������� ���� �� ����

    vector<Vertex> vec; // ������ ������ ���� ����

    Vertex v; // ���� ����ü �ν��Ͻ�

    // �ϱ� ���� ����
    v.pos = Vector3(0.0f, radius, 0.0f); // �ϱ� ������ ��ġ
    v.uv = Vector2(0.5f, 0.0f); // �ϱ� ������ �ؽ�ó ��ǥ
    v.normal = v.pos; // ���� ���� ����
    v.normal.Normalize(); // ���� ���� ����ȭ
    v.tangent = Vector3(1.0f, 0.0f, 1.0f); // ���� ���� ����
    vec.push_back(v); // ���Ϳ� �ϱ� ���� �߰�

    float stackAngle = XM_PI / stackCount; // ���� ���� ����
    float sliceAngle = XM_2PI / sliceCount; // �����̽� ���� ����

    float deltaU = 1.f / static_cast<float>(sliceCount); // U ��ǥ�� ��ȭ��
    float deltaV = 1.f / static_cast<float>(stackCount); // V ��ǥ�� ��ȭ��

    // �� ���ø��� ������ ��� (�ϱذ� ������ ������ �κ�)
    for (UINT32 y = 1; y <= stackCount - 1; ++y)
    {
        float phi = y * stackAngle; // ���� ������ ����

        // �� �����̽����� ������ ���
        for (UINT32 x = 0; x <= sliceCount; ++x)
        {
            float theta = x * sliceAngle; // ���� �����̽��� ����

            // ������ ��ġ ���
            v.pos.x = radius * sinf(phi) * cosf(theta);
            v.pos.y = radius * cosf(phi);
            v.pos.z = radius * sinf(phi) * sinf(theta);

            // �ؽ�ó ��ǥ ���
            v.uv = Vector2(deltaU * x, deltaV * y);

            // ���� ���� ��� �� ����ȭ
            v.normal = v.pos;
            v.normal.Normalize();

            // ���� ���� ��� �� ����ȭ
            v.tangent.x = -radius * sinf(phi) * sinf(theta);
            v.tangent.y = 0.0f;
            v.tangent.z = radius * sinf(phi) * cosf(theta);
            v.tangent.Normalize();

            // ������ ���Ϳ� �߰�
            vec.push_back(v);
        }
    }

    // ���� ���� ����
    v.pos = Vector3(0.0f, -radius, 0.0f); // ���� ������ ��ġ
    v.uv = Vector2(0.5f, 1.0f); // ���� ������ �ؽ�ó ��ǥ
    v.normal = v.pos; // ���� ���� ����
    v.normal.Normalize(); // ���� ���� ����ȭ
    v.tangent = Vector3(1.0f, 0.0f, 0.0f); // ���� ���� ����
    vec.push_back(v); // ���Ϳ� ���� ���� �߰�

    vector<UINT32> idx; // �ε����� ������ ���� ����

    // �ϱ� �ε��� ����
    for (UINT32 i = 0; i <= sliceCount; ++i)
    {
        // �ϱؿ��� �� �����̽��� ����Ǵ� �ﰢ���� ����
        idx.push_back(0); // �ϱ� ���� �ε���
        idx.push_back(i + 2); // ���� �����̽��� ���� ���� �ε���
        idx.push_back(i + 1); // ���� �����̽��� ���� �ε���
    }

    // ���� �ε��� ����
    UINT32 ringVertexCount = sliceCount + 1; // �� ������ �ִ� ������ ��
    for (UINT32 y = 0; y < stackCount - 2; ++y)
    {
        for (UINT32 x = 0; x < sliceCount; ++x)
        {
            // ���� ������ ���� ������ �����ϴ� �� ���� �ﰢ���� ����
            idx.push_back(1 + y * ringVertexCount + x); // ���� ������ ���� �����̽� ����
            idx.push_back(1 + y * ringVertexCount + x + 1); // ���� ������ ���� �����̽� ����
            idx.push_back(1 + (y + 1) * ringVertexCount + x); // ���� ������ ���� �����̽� ����

            idx.push_back(1 + (y + 1) * ringVertexCount + x); // ���� ������ ���� �����̽� ����
            idx.push_back(1 + y * ringVertexCount + x + 1); // ���� ������ ���� �����̽� ����
            idx.push_back(1 + (y + 1) * ringVertexCount + x + 1); // ���� ������ ���� �����̽� ����
        }
    }

    // ���� �ε��� ����
    UINT32 bottomIndex = static_cast<UINT32>(vec.size()) - 1; // ���� ������ �ε���
    UINT32 lastRingStartIndex = bottomIndex - ringVertexCount; // ������ ������ ���� ���� �ε���
    for (UINT32 i = 0; i < sliceCount; ++i)
    {
        // ���ؿ��� �� �����̽��� ����Ǵ� �ﰢ���� ����
        idx.push_back(bottomIndex); // ���� ���� �ε���
        idx.push_back(lastRingStartIndex + i); // ������ ������ ���� �����̽� ����
        idx.push_back(lastRingStartIndex + i + 1); // ������ ������ ���� �����̽� ����
    }

    // ���ο� �޽� ��ü ���� �� �ʱ�ȭ
    shared_ptr<Mesh> mesh = make_shared<Mesh>();
    mesh->Init(vec, idx);

    // "Sphere"��� Ű�� �޽��� ���ҽ��� �߰�
    Add(L"Sphere", mesh);

    return mesh; // �޽� ��ȯ
}


shared_ptr<Mesh> Resources::LoadCubeMesh()
{
	//�̹� "Cube"��� Ű�� �ε�� �Ž��� �ִ��� Ȯ��
	shared_ptr<Mesh> findMesh = GetResource<Mesh>(L"Cube");

	//�̹� ������ ��ȯ
	if (findMesh)
		return findMesh;

	//ť���� ���� ũ�� ����
	float wHalf = 0.5f;
	float hHalf = 0.5f;
	float dHalf = 0.5f;

	//���� �迭 ����(24���� ����) //�� ������
	vector<Vertex> vec(24);

	//�ո� ���� ����
	vec[0] = Vertex(Vector3(-wHalf, -hHalf, -dHalf), Vector2(0.0f, 1.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f));
	vec[1] = Vertex(Vector3(-wHalf, +hHalf, -dHalf), Vector2(0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f));
	vec[2] = Vertex(Vector3(+wHalf, +hHalf, -dHalf), Vector2(1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f));
	vec[3] = Vertex(Vector3(+wHalf, -hHalf, -dHalf), Vector2(1.0f, 1.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f));

	//�޸� ���� ����
	vec[4] = Vertex(Vector3(-wHalf, -hHalf, +dHalf), Vector2(1.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f));
	vec[5] = Vertex(Vector3(+wHalf, -hHalf, +dHalf), Vector2(0.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f));
	vec[6] = Vertex(Vector3(+wHalf, +hHalf, +dHalf), Vector2(0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f));
	vec[7] = Vertex(Vector3(-wHalf, +hHalf, +dHalf), Vector2(1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f));

	//���� ���� ����
	vec[8]  = Vertex(Vector3(-wHalf, +hHalf, -dHalf), Vector2(0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f));
	vec[9]  = Vertex(Vector3(-wHalf, +hHalf, +dHalf), Vector2(0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f));
	vec[10] = Vertex(Vector3(+wHalf, +hHalf, +dHalf), Vector2(1.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f));
	vec[11] = Vertex(Vector3(+wHalf, +hHalf, -dHalf), Vector2(1.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f));

	//�Ʒ��� ���� ����
	vec[12] = Vertex(Vector3(-wHalf, -hHalf, -dHalf), Vector2(1.0f, 1.0f), Vector3(0.0f, -1.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f));
	vec[13] = Vertex(Vector3(+wHalf, -hHalf, -dHalf), Vector2(0.0f, 1.0f), Vector3(0.0f, -1.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f));
	vec[14] = Vertex(Vector3(+wHalf, -hHalf, +dHalf), Vector2(0.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f));
	vec[15] = Vertex(Vector3(-wHalf, -hHalf, +dHalf), Vector2(1.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f));

	//���ʸ� ���� ����
	vec[16] = Vertex(Vector3(-wHalf, -hHalf, +dHalf), Vector2(0.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f));
	vec[17] = Vertex(Vector3(-wHalf, +hHalf, +dHalf), Vector2(0.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f));
	vec[18] = Vertex(Vector3(-wHalf, +hHalf, -dHalf), Vector2(1.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f));
	vec[19] = Vertex(Vector3(-wHalf, -hHalf, -dHalf), Vector2(1.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f));

	//�����ʸ� ���� ����
	vec[20] = Vertex(Vector3(+wHalf, -hHalf, -dHalf), Vector2(0.0f, 1.0f), Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f));
	vec[21] = Vertex(Vector3(+wHalf, +hHalf, -dHalf), Vector2(0.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f));
	vec[22] = Vertex(Vector3(+wHalf, +hHalf, +dHalf), Vector2(1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f));
	vec[23] = Vertex(Vector3(+wHalf, -hHalf, +dHalf), Vector2(1.0f, 1.0f), Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f));

	//�ε��� �迭 ���� (36���� �ε���)
	vector<UINT32> idx(36);

	//�ո� �ε��� ����
	idx[0] = 0; idx[1] = 1; idx[2] = 2;
	idx[3] = 0; idx[4] = 2; idx[5] = 3;
	//�ڸ� �ε��� ����
	idx[6] = 4; idx[7] = 5; idx[8] = 6;
	idx[9] = 4; idx[10] = 6; idx[11] = 7;
	//���� �ε��� ����
	idx[12] = 8; idx[13] = 9; idx[14] = 10;
	idx[15] = 8; idx[16] = 10; idx[17] = 11;
	//�Ʒ��� �ε��� ����
	idx[18] = 12; idx[19] = 13; idx[20] = 14;
	idx[21] = 12; idx[22] = 14; idx[23] = 15;
	//���ʸ� �ε��� ����
	idx[24] = 16; idx[25] = 17; idx[26] = 18;
	idx[27] = 16; idx[28] = 18; idx[29] = 19;
	//�����ʸ� �ε��� ����
	idx[30] = 20; idx[31] = 21; idx[32] = 22;
	idx[33] = 20; idx[34] = 22; idx[35] = 23;

	//���ο� �Ž� ��ü ���� �� �ʱ�ȭ
	shared_ptr<Mesh> mesh = make_shared<Mesh>();
	mesh->Init(vec, idx);

	//"Cube"��� Ű�� �Ž��� ���ҽ��� �߰�
	Add(L"Cube", mesh);

	return mesh;
}