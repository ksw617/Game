#include "pch.h"
#include "ViewFrustum.h"
#include "Camera.h"	 // ī�޶� �� ���, ���� ��� �ޱ� ����

void ViewFrustum::FinalUpdate()
{
	//ī�޶��� �� ����� ����ķ� ��ȯ
	Matrix matViewInvert = Camera::StaticMatrixView.Invert();

	//ī�޶� ���� ����� ����ķ� ��ȯ
	Matrix matProjectionInvert = Camera::StaticMatrixProjection.Invert();

	//����ĵ��� ���Ͽ� ��ȯ ����� ���
	Matrix matInvert = matProjectionInvert * matViewInvert;

	//ȭ�� ��ǥ���� ���� ��ǥ�� ��ȯ�� 8���� �ڳ� ����Ʈ�� ���
	vector<Vector3> worldPos =
	{
		XMVector3TransformCoord(Vector3(-1.f, 1.f, 0.f), matInvert),	//����� Top-Left
		XMVector3TransformCoord(Vector3(1.f, 1.f, 0.f), matInvert),		//����� Top-Right
		XMVector3TransformCoord(Vector3(1.f, -1.f, 0.f), matInvert),	//����� Bottom-Right
		XMVector3TransformCoord(Vector3(-1.f, -1.f, 0.f), matInvert),	//����� Bottom-Left

		XMVector3TransformCoord(Vector3(-1.f, 1.f, 1.f), matInvert),	//�� Top-Left
		XMVector3TransformCoord(Vector3(1.f, 1.f, 1.f), matInvert),		//�� Top-Right
		XMVector3TransformCoord(Vector3(1.f, -1.f, 1.f), matInvert),	//�� Bottom-Right
		XMVector3TransformCoord(Vector3(-1.f, -1.f, 1.f), matInvert),	//�� Bottom-Left

	};

	//�� ����� 3���� ����Ʈ�� �����Ͽ� ���
	planes[PLANE_FRONT] = XMPlaneFromPoints(worldPos[0], worldPos[1], worldPos[2]);	// Near Plane(�ð����)
	planes[PLANE_BACK] = XMPlaneFromPoints(worldPos[4], worldPos[7], worldPos[5]);	// Far Plane(�ݽð����)
	planes[PLANE_UP] = XMPlaneFromPoints(worldPos[4], worldPos[5], worldPos[1]);	// Top Plane(�ð����)
	planes[PLANE_DOWN] = XMPlaneFromPoints(worldPos[7], worldPos[3], worldPos[6]);	// Bottom Plane(�ݽð����)
	planes[PLANE_LEFT] = XMPlaneFromPoints(worldPos[4], worldPos[0], worldPos[7]);	// Left Plane(�ð����)
	planes[PLANE_RIGHT] = XMPlaneFromPoints(worldPos[5], worldPos[6], worldPos[1]);	// Right Plane(�ݽð����)

}

bool ViewFrustum::ContainsSphere(const Vector3& pos, float radius)
{
	//��ü�� �����ȿ� �����ϰų� �����ִ��� üũ
	for (const Vector4& plane : planes)
	{
		//����� normal ���� ����
		Vector3 normal = Vector3(plane.x, plane.y, plane.z);

		//��� ������(ax + by + cz + d)�� �������� ����Ͽ� ��ü�� ����� ������� Ȯ��
		if (normal.Dot(pos) + plane.w > radius)
			return false; // ���� ���ٸ� false ��ȯ
	}

	//��� ����� ����ϸ� true�� ��ȯ
	return true;
}
