#pragma once
//Singleton ���ø� Ŭ�� ����
template<typename T>
class Singleton
{
protected:
	Singleton() = default;
	~Singleton() = default;
public:
	//Singleton �ν��Ͻ� ��ȯ
	static T& Get()
	{
		static T instnace;
		return instnace;
	}

public:
	//���� ������ ����
	Singleton(const Singleton&) = delete;
	//���� �Ҵ� ������ ����
	Singleton& operator=(const Singleton&) = delete;
};

