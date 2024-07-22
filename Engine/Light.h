#pragma once
#include "Component.h"

//LIGHT_TYPE ������ ����: ������ ������ ��Ÿ��
enum class LIGHT_TYPE : UINT8
{
	DIRECTIONAL_LIGHT,	//���⼺ ����
	POINT_LIGHT,		//������ ����
	SPOT_LIGHT,			//���� ����
};
				 
//LightColor ����ü ����: ������ ���� �Ӽ����� ����
struct LightColor
{
	Vector4 diffuse;	// Ȯ�걤 ����
	Vector4 ambient;	// �ֺ��� ����
	Vector4 specular;	// �ݻ籤 ����

};

//LightInfo ����ü ����: ������ �Ҽ����� ����
struct LightInfo
{
	LightColor color;	//���� ����	 //16+16+16
	Vector4 position;	//���� ��ġ	 //16
	Vector4 direction;	//���� ����	 //16
	INT32 lightType;	//���� ����	 //4
	float range;		//���� ����	 //4
	float angle;		//���� ����	 //4
	INT32 padding;		//�е�		 //4

};

//LightParams ����ü ����: �ټ��� ���� ������ ����
struct LightParams
{
	UINT32 lightCount;		//���� ����
	Vector3	padding;		//�е�
	LightInfo lights[50];	//���� ���� �迭
};

class Light : public Component
{
public:
	Light() : Component(COMPONENT_TYPE::LIGHT) {} // ������
	virtual ~Light() {}	//�Ҹ���
private:
	LightInfo lightInfo = {};	//���� ���� �ʱ�ȭ
public:
	//Get �Լ�
	//���� ����
	const LightInfo& GetLightInfo() { return lightInfo; }
public:
	//Set �Լ�
	//���� ����
	void SetLightDirection(const Vector3& direction) { lightInfo.direction = Vec3ToVec4(direction); }
	//Ȯ�걤 ����
	void SetDiffuse(const Vector3& diffuse) { lightInfo.color.diffuse = Vec3ToVec4(diffuse); }
	//�ֺ��� ����
	void SetAmbient(const Vector3& ambient) { lightInfo.color.ambient = Vec3ToVec4(ambient); }
	//�ݻ籤 ����
	void SetSpecular(const Vector3& specular) { lightInfo.color.specular = Vec3ToVec4(specular); }
	//���� Ÿ�� 
	void SetLightType(LIGHT_TYPE tpye) { lightInfo.lightType = static_cast<INT32>(type); }

	//���� ���� 
	void SetLightRange(float range) { lightInfo.range = range; }
	//���� ����
	void SetLightAngle(float angle) { lightInfo.angle = angle; }
public:
	//���� ������Ʈ �Լ�(Component�� �������̵�)
	virtual void FinalUpdate() override;
private:
	//Vector3�� Vector4�� ��ȯ
	Vector4 Vec3ToVec4(const Vector3& vec3) { return Vector4(vec3.x, vec3.y, vec3.z, 0.f); }
};

