#pragma once
class MainGame
{
public:
	//초기화
	void Init(HWND hwnd, int width, int height, bool windowed);
	//프로그램진행
	void Tick();
};

