#pragma once
#include "Component.h"

enum class PROJECTION_TYPE : UINT8
{	
	PERSPECTIVE,	// ���� ����
	ORTHOGRAPHIC,	// ���� ����

};

class Camera : public Component
{
private:
	PROJECTION_TYPE type = PROJECTION_TYPE::PERSPECTIVE; // ī�޶� Ÿ��(�⺻���� ���� ����)

	float nearView = 0.3f;	//�ٰ� ��� �Ÿ�
	float farView = 1000.f;	//���� ��� �Ÿ�
	float fov = XM_PI / 4.f;//�þ߰� (FOV)
	float size = 1.f;		//ũ�Ⱚ (���� ������ ���)

	Matrix matrixView = {};			//�� ���
	Matrix matrixProjection = {};	//���� ���
public:
	//�ӽ÷� ���Ǵ� ���� ������-> MainCamera
	static Matrix StaticMatrixView;			//���� �� ���
	static Matrix StaticMatrixProjection;	//���� ���� ���
public:
	//������
	Camera();
	//�Ҹ���
	virtual ~Camera();

public:
	// ���� ������Ʈ �Լ�(�� ������ ȣ���)
	virtual void FinalUpdate() override;
};

