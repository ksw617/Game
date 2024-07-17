#pragma once
//Singleton 템플릿 클라스 정의
template<typename T>
class Singleton
{
protected:
	Singleton() = default;
	~Singleton() = default;
public:
	//Singleton 인스턴스 반환
	static T& Get()
	{
		static T instnace;
		return instnace;
	}

public:
	//복사 생성자 삭제
	Singleton(const Singleton&) = delete;
	//복사 할당 연산자 삭제
	Singleton& operator=(const Singleton&) = delete;
};

