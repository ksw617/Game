#include "pch.h"
#include "Input.h"

void Input::Init(HWND _hwnd)
{
	hwnd = _hwnd;		//윈도우 핸들 등록
	
	//키 상태 벡터를 초기화 하고 모든 키 상태를 NONE으로 설정
	states.resize(KEY_TYPE_COUNT, KEY_STATE::NONE);
}

void Input::Update()
{
	//현재 활성화된 윈도우 핸들을 가져옴
	HWND activeHwnd = GetActiveWindow();

	//입력을 받을 윈도가 활성 윈도가 아니면 
	if (hwnd != activeHwnd)
	{
		//모든 키 상태를 NONE으로 설정
		for (UINT32 key = 0; key < KEY_TYPE_COUNT; key++)
		{
			states[key] = KEY_STATE::NONE;
		}

		return;
	}

	//현재 키보드 상태를 가져옴
	BYTE asciiKeys[KEY_TYPE_COUNT] = {};

	//키값이 없을 경우
	if (GetKeyboardState(asciiKeys) == false)
		return;

	//모든 키 타입을 순회하며 상태를 업데이트
	for (UINT32 key = 0; key < KEY_TYPE_COUNT; key++)
	{
		//키 상태를 가져옴
		KEY_STATE& state = states[key];

		//키가 눌려 있는지 확인
		if (asciiKeys[key] & 0x80)
		{
			
			//이전 프레임 키를 누른 상태라면 PRESS, 그렇지 않으면 DOWN
			if (state == KEY_STATE::PRESS || state == KEY_STATE::DOWN)
			{
				state = KEY_STATE::PRESS;
			}
			else
			{
				state = KEY_STATE::DOWN;
			}

		}else // 키가 눌려 있지 않은 상태라면
		{
			//이전 프레임에서 키를 누른 상태라면 UP, 그렇지 않으면 NONE
			if (state == KEY_STATE::PRESS || state == KEY_STATE::DOWN)
			{
				state = KEY_STATE::UP;
			}
			else
			{
				state = KEY_STATE::NONE;
			}
		}

	}

}
