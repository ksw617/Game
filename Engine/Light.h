#pragma once
#include "Component.h"

enum class LIGHT_TYPE : UINT8
{
	DIRECTIONAL_LIGHT,
	POINT_LIGHT,		
	SPOT_LIGHT,			
};
				 

struct LightColor
{
	Vector4 diffuse;	
	Vector4 ambient;	
	Vector4 specular;	

};

struct LightInfo
{
	LightColor color;	
	Vector4 position;	
	Vector4 direction;	
	INT32 lightType;	
	float range;		
	float angle;		
	INT32 padding;		

};

struct LightParams
{
	UINT32 lightCount;		
	Vector3	padding;		
	LightInfo lights[50];	
};

class Light : public Component
{
public:
	Light() : Component(COMPONENT_TYPE::LIGHT) {}
	virtual ~Light() {}	
private:
	LightInfo lightInfo = {};	
public:
	const LightInfo& GetLightInfo() { return lightInfo; }
public:
	void SetLightDirection(const Vector3& direction) { lightInfo.direction = Vec3ToVec4(direction); }
	void SetDiffuse(const Vector3& diffuse) { lightInfo.color.diffuse = Vec3ToVec4(diffuse); }
	void SetAmbient(const Vector3& ambient) { lightInfo.color.ambient = Vec3ToVec4(ambient); }
	void SetSpecular(const Vector3& specular) { lightInfo.color.specular = Vec3ToVec4(specular); }
	//오타 수정
	void SetLightType(LIGHT_TYPE type) { lightInfo.lightType = static_cast<INT32>(type); }

	void SetLightRange(float range) { lightInfo.range = range; }
	void SetLightAngle(float angle) { lightInfo.angle = angle; }
public:
	virtual void FinalUpdate() override;
private:
	Vector4 Vec3ToVec4(const Vector3& vec3) { return Vector4(vec3.x, vec3.y, vec3.z, 0.f); }
};

