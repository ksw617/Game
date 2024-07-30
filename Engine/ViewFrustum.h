#pragma once

//평면의 탕닙을 정의하는 열거형
enum PLANE_TYPE : UINT8
{
	PLANE_FRONT,	//전방 평면
	PLANE_BACK,		//후방 평면
	PLANE_UP,		//상단 평면
	PLANE_DOWN,		//하단 평면
	PLANE_LEFT,		//좌측 평면
	PLANE_RIGHT,	//우측 평면

	PLANE_END,		//평면의 총 갯수 

};

class ViewFrustum
{
private:
	//각 평면의 값을 저장하는 배열
	array<Vector4, PLANE_END> planes;

public:
	//최종 업데이트 함수: 뷰 행렬과 투영 행렬을 사용하여 평면을 계산
	void FinalUpdate();
public:
	//구체가 ViewFrustum 내에 있는지 확인 하는 함수
	bool ContainsSphere(const Vector3& pos, float radius);
};

