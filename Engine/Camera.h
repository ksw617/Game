#pragma once
#include "Component.h"
#include "ViewFrustum.h" 

enum class PROJECTION_TYPE : UINT8
{	
	PERSPECTIVE,	
	ORTHOGRAPHIC,	

};

class Camera : public Component
{
private:
	PROJECTION_TYPE projectionType = PROJECTION_TYPE::PERSPECTIVE; 

	float nearView = 0.3f;	
	float farView = 1000.f;	
	float fov = XM_PI / 4.f;
	float size = 1.f;		

	Matrix matrixView = {};			
	Matrix matrixProjection = {};	

	ViewFrustum viewFrustum;	

	UINT32 cullingMask = 0;

public:
	static Matrix StaticMatrixView;			
	static Matrix StaticMatrixProjection;	
private:
	//Deferred용 Forward용 GameObject 각각 담을 vector
	vector<shared_ptr<GameObject>> deferredObjs;
	vector<shared_ptr<GameObject>> forwardObjs;
public:
	Camera();
	virtual ~Camera();

public:
	virtual void FinalUpdate() override;	 
public:	
	//바로 랜더 안걸고
	//void Render();
	//정렬 먼저 하고
	void SortGameObject();
	//Deferred 용 랜더 함수
	void RenderDeferred();
	//Forward 용 랜더 함수
	void RenderForward();

public:
	void SetProjectionType(PROJECTION_TYPE type) { projectionType = type; }
	PROJECTION_TYPE GetProjectionType() const { return projectionType; }

public:
	void SetCullingMaskAll() { SetCullingMask(UINT32_MAX); }
	void ClearCullingMask() { SetCullingMask(0); }
	void SetCullingMask(UINT32 mask) { cullingMask = mask; }
	void SetCullingMaskLayerOnOff(UINT8 layer, bool on)
	{
		if (on)
			cullingMask |= (1 << layer);
		else
			cullingMask &= ~(1 << layer);
	}

	bool IsCulled(UINT8 layer) const { return (cullingMask & (1 << layer)) != 0; }
};

