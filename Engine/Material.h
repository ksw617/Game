#pragma once
#include "Object.h"

class Shader;
class Texture;


enum
{
	MATERIAL_INT_COUNT = 5,		
	MATERIAL_FLOAT_COUNT = 5,	
	MATERIAL_TEXTURE_COUNT = 5, 

};


struct MaterialParams
{

	array<INT32, MATERIAL_INT_COUNT> intParams;		
	array<float, MATERIAL_FLOAT_COUNT> floatParams; 
	//Texture on off 파라미터들
	array<INT32, MATERIAL_TEXTURE_COUNT> textureOnParams;

	void SetInt(UINT8 index, INT32 value) { intParams[index] = value; }
	void SetFloat(UINT8 index, float value) { floatParams[index] = value; }

	//Texture On Off Set 함수
	void SetTextureOn(UINT8 index, INT32 value) { textureOnParams[index] = value; }

};

class Material : public Object
{
private:
	shared_ptr<Shader> shader;	
	MaterialParams params;		
	array<shared_ptr<Texture>, MATERIAL_TEXTURE_COUNT> textures;	
public:
	//타입 초기화
	Material() : Object(OBJECT_TYPE::MATERIAL) {}
	virtual ~Material() {}

public:
	shared_ptr<Shader> GetShader() { return shader; }
public:
	void SetShader(shared_ptr<Shader> _shader) { shader = _shader; }
	void SetInt(UINT8 index, INT32 value) { params.SetInt(index, value); }
	void SetFloat(UINT8 index, float value) { params.SetFloat(index, value); }
	void SetTexture(UINT8 index, shared_ptr<Texture> texture)
	{
		textures[index] = texture;

		//texture가 nullptr이라면 0 아니라면 1
		params.SetTextureOn(index, texture == nullptr ? 0 : 1);
	}
public:
	void Update();
};

