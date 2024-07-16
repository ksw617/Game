#pragma once

//Object 타입을 정의하는 열거형
enum class OBJECT_TYPE : UINT8
{
	NONE,		//기본값
	GAMEOBJECT, //게임 오브젝트
	COMPONENT,	//컴포넌트
	MATERIAL,	//매테리얼
	MESH,		//매쉬
	SHADER,		//쉐이더
	TEXTURE,	//텍스처

	END,		//끝
};

//Object 타입의 갯수를 정의
enum
{
	OBJECT_TYPE_COUNT = static_cast<UINT8>(OBJECT_TYPE::END)
};

class Object
{
protected:
	OBJECT_TYPE type = OBJECT_TYPE::NONE;	//Object의 타입
	wstring name;							//Object의 이름
public:
	//생성자 : Object 타입을 설정
	Object(OBJECT_TYPE _type) : type(_type) {}

	//소멸자
	virtual ~Object() {}

public:
	//타입 반환하는 Get 함수
	OBJECT_TYPE GetType() const { return type; }
	//이름 반환하는 Get 함수
	const wstring& GetName() { return name; }
public:
	//이름 설정 Set 함수
	void SetName(const wstring& _name) { name = _name; }
protected:
	//객체를 로드하는 가상 함수
	virtual void Load(const wstring& path) {}
	//객체를 저장하는 가상 함수
	virtual void Save(const wstring& path) {}


};

