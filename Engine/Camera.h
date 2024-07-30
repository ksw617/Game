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
	//type에서 	projectionType 으로 변환
	PROJECTION_TYPE projectionType = PROJECTION_TYPE::PERSPECTIVE; 

	float nearView = 0.3f;	
	float farView = 1000.f;	
	float fov = XM_PI / 4.f;
	float size = 1.f;		

	Matrix matrixView = {};			
	Matrix matrixProjection = {};	

	ViewFrustum viewFrustum;	

	//컬링 마스크 index
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
	//카메라 투영 타입을 설정하는 set 함수
	void SetProjectionType(PROJECTION_TYPE type) { projectionType = type; }
	//카메라의 투영 타입을 반환하는 get 함수
	PROJECTION_TYPE GetProjectionType() { return projectionType; }

public:
	//모든 레이어의 컬링 마스크를 1로 설정하는 함수 1111 1111 1111 1111 1111 1111 1111 1111
	void SetCullingMaskAll() { SetCullingMask(UINT32_MAX); }
	//모든 레이어의 컬링 마스크를 0으로설정하는 함수 0000 0000 0000 0000 0000 0000 0000 0000
	void ClearCullingMask() { SetCullingMask(0); }
	//특정 마스크 값을 설정 하는 함수
	void SetCullingMask(UINT32 mask) { cullingMask = mask; }
	//특정 레이어의 컬링마스크를 켜거나 끄는 함수
	void SetCullingMaskLayerOnOff(UINT8 layer, bool on)
	{
		if (on)
			cullingMask |= (1 << layer); //해당 레이어를 킴
		else
			cullingMask &= ~(1 << layer);//해당 레이어를 끔
	}

	//특정 레이어가 컬링되었는지 확인 하는 함수
	bool IsCulled(UINT8 layer) const { return (cullingMask & (1 << layer)) != 0; }
};

