#pragma once
				   
//키 타입을 정의하는 열거형 클라스
enum class KEY_TYPE
{
	UP = VK_UP,			//위쪽 방향 키
	DOWN = VK_DOWN,		//아래쪽 방향키 
	LEFT = VK_LEFT,		//왼쪽 방향키
	RIGHT = VK_RIGHT,	//오른쪽 방향키

	W = 'W',			//'W' 키
	A = 'A',			//'A' 키
	S = 'S',			//'S' 키
	D = 'D',			//'D' 키

};

//키 상태를 정의하는 열거형 클라스
enum class KEY_STATE
{
	NONE,	//아무 상태 아님
	PRESS,	//키를 누르고 있는 상태
	DOWN,	//키를 처음 누른 상태
	UP,		//키를 눌렀다 뗀	상태
	END,	//상태 끝
};

//키와 상태의 갯수를 정의 하는 열거형
enum
{
	KEY_TYPE_COUNT = static_cast<int>(UINT8_MAX + 1),	// 키 타입의 총 갯수
	KEY_STATE_COUNT = static_cast<int>(KEY_STATE::END), // 키 상태의 총 갯수
};

class Input
{
private:
	HWND hwnd;	//윈도 핸들
	vector<KEY_STATE> states; // 키상태를 저장 하는 벡터

private:
	//키의 현재 상태를 가져오는 함수
	inline KEY_STATE GetState(KEY_TYPE key) { return states[static_cast<UINT8>(key)]; }
public:
	//키가 눌려져 있는지 확인 하는 함수
	bool GetButton(KEY_TYPE key) { return GetState(key) == KEY_STATE::PRESS; }

	//키가 처음 눌렸는지 확인 하는 함수
	bool GetButtonDown(KEY_TYPE key) { return GetState(key) == KEY_STATE::DOWN; }
	//키가 눌렸다가 떼졌는지 확인 하는 함수
	bool GetButtonUp(KEY_TYPE key) { return GetState(key) == KEY_STATE::UP; }
public:
	//입력 클라스를 초기화
	void Init(HWND _hwnd);

	//키 상태를 업데이트 하는 함수
	void Update();

};

