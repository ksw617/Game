#pragma once
#include "Component.h"
#include "ViewFrustum.h" // ViewFrustum incule

enum class PROJECTION_TYPE : UINT8
{	
	PERSPECTIVE,	
	ORTHOGRAPHIC,	

};

class Camera : public Component
{
private:
	//type���� 	projectionType ���� ��ȯ
	PROJECTION_TYPE projectionType = PROJECTION_TYPE::PERSPECTIVE; 

	float nearView = 0.3f;	
	float farView = 1000.f;	
	float fov = XM_PI / 4.f;
	float size = 1.f;		

	Matrix matrixView = {};			
	Matrix matrixProjection = {};	

	ViewFrustum viewFrustum;	

	//�ø� ����ũ index
	UINT32 cullingMask = 0;

public:
	static Matrix StaticMatrixView;			
	static Matrix StaticMatrixProjection;	
public:
	Camera();
	virtual ~Camera();

public:
	virtual void FinalUpdate() override;	 
public:			 
	void Render();

public:
	//ī�޶� ���� Ÿ���� �����ϴ� set �Լ�
	void SetProjectionType(PROJECTION_TYPE type) { projectionType = type; }
	//ī�޶��� ���� Ÿ���� ��ȯ�ϴ� get �Լ�
	PROJECTION_TYPE GetProjectionType() { return projectionType; }

public:
	//��� ���̾��� �ø� ����ũ�� 1�� �����ϴ� �Լ� 1111 1111 1111 1111 1111 1111 1111 1111
	void SetCullingMaskAll() { SetCullingMask(UINT32_MAX); }
	//��� ���̾��� �ø� ����ũ�� 0���μ����ϴ� �Լ� 0000 0000 0000 0000 0000 0000 0000 0000
	void ClearCullingMask() { SetCullingMask(0); }
	//Ư�� ����ũ ���� ���� �ϴ� �Լ�
	void SetCullingMask(UINT32 mask) { cullingMask = mask; }
	//Ư�� ���̾��� �ø�����ũ�� �Ѱų� ���� �Լ�
	void SetCullingMaskLayerOnOff(UINT8 layer, bool on)
	{
		if (on)
			cullingMask |= (1 << layer); //�ش� ���̾ Ŵ
		else
			cullingMask &= ~(1 << layer);//�ش� ���̾ ��
	}

	//Ư�� ���̾ �ø��Ǿ����� Ȯ�� �ϴ� �Լ�
	bool IsCulled(UINT8 layer) const { return (cullingMask & (1 << layer)) != 0; }
};

