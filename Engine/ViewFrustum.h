#pragma once

//����� ������ �����ϴ� ������
enum PLANE_TYPE : UINT8
{
	PLANE_FRONT,	//���� ���
	PLANE_BACK,		//�Ĺ� ���
	PLANE_UP,		//��� ���
	PLANE_DOWN,		//�ϴ� ���
	PLANE_LEFT,		//���� ���
	PLANE_RIGHT,	//���� ���

	PLANE_END,		//����� �� ���� 

};

class ViewFrustum
{
private:
	//�� ����� ���� �����ϴ� �迭
	array<Vector4, PLANE_END> planes;

public:
	//���� ������Ʈ �Լ�: �� ��İ� ���� ����� ����Ͽ� ����� ���
	void FinalUpdate();
public:
	//��ü�� ViewFrustum ���� �ִ��� Ȯ�� �ϴ� �Լ�
	bool ContainsSphere(const Vector3& pos, float radius);
};

