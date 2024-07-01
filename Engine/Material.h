#pragma once

class Shader;
class Texture;

//Material 매개변수의 최대 갯수를 정의
enum
{
	MATERIAL_INT_COUNT = 5,		//정수 매개변수의 최대 갯수
	MATERIAL_FLOAT_COUNT = 5,	//실수 매개변수의 최대 갯수 
	MATERIAL_TEXTURE_COUNT = 5, //텍스처 매개변수의 최대 갯수

};

//Material 매개변수를 저장하는 구조체
struct MaterialParams
{
	//정수 매개변수를 저장하는 배열
	array<INT32, MATERIAL_INT_COUNT> intParams;		//[4][4][4][4][4]
	array<float, MATERIAL_FLOAT_COUNT> floatParams; //[4][4][4][4][4]

	//정수 매개변수를 설정하는 Set 함수
	void SetInt(UINT8 index, INT32 value) { intParams[index] = value; }
	//실수 매개변수를 설정하는 Set 함수
	void SetFloat(UINT8 index, float value) { floatParams[index] = value; }


};

class Material
{
private:
	shared_ptr<Shader> shader;	//쉐이더 객체
	MaterialParams params;		//Material 매개변수
	array<shared_ptr<Texture>, MATERIAL_TEXTURE_COUNT> textures;	//텍스처 배열

public:
	//쉐이더를 반환하는 Get 함수
	shared_ptr<Shader> GetShader() { return shader; }
public:
	//쉐이더를 Set하는 함수
	void SetShader(shared_ptr<Shader> _shader) { shader = _shader; }
	
	//정수 매개변수를 Set하는 함수
	void SetInt(UINT8 index, INT32 value) { params.SetInt(index, value); }
	//실수 매개변수를 Set하는 함수
	void SetFloat(UINT8 index, float value) { params.SetFloat(index, value); }
	//실수 매개변수를 Set하는 함수
	void SetTexture(UINT8 index, shared_ptr<Texture> texture) { textures[index] = texture; }
public:
	//Material 업데이트 하는 함수
	void Update();
};

