#pragma once
				   
enum class KEY_TYPE
{
	UP = VK_UP,
	DOWN = VK_DOWN,
	LEFT = VK_LEFT,
	RIGHT = VK_RIGHT,

	W = 'W',
	A = 'A',
	S = 'S',
	D = 'D',

	//키코드 추가
	Q = 'Q',
	E = 'E',

};

enum class KEY_STATE
{
	NONE,	
	PRESS,	
	DOWN,	
	UP,		
	END,	
};


enum
{
	KEY_TYPE_COUNT = static_cast<int>(UINT8_MAX + 1),	
	KEY_STATE_COUNT = static_cast<int>(KEY_STATE::END), 
};

class Input
{
private:
	HWND hwnd;	
	vector<KEY_STATE> states;

private:
	inline KEY_STATE GetState(KEY_TYPE key) { return states[static_cast<UINT8>(key)]; }
public:
	bool GetButton(KEY_TYPE key) { return GetState(key) == KEY_STATE::PRESS; }
	bool GetButtonDown(KEY_TYPE key) { return GetState(key) == KEY_STATE::DOWN; }
	bool GetButtonUp(KEY_TYPE key) { return GetState(key) == KEY_STATE::UP; }
public:
	void Init(HWND _hwnd);
	void Update();

};

