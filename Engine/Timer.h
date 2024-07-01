#pragma once
class Timer
{
private:
	UINT64 frequency = 0;	//	타이머의 빈도를 나타냄
	UINT64 prevCount = 0;	//	이전 카운트 값
	float deltaTime = 0.f;	//	프레임 간 시간 간격

private:
	UINT32 frameCount = 0;	// 프레임 카운트
	float frameTime = 0.f;	// 프레임 시간 누적값
	UINT32 fps = 0;			//초당 프레임 수(Frame Per Second)

public:
	//현재 FPS 값을 반환하는 함수
	UINT32 GetFPS() const { return fps; }
	//프레임 간 시간 간격을 반환하는 함수
	float GetDeltaTime() const { return deltaTime; }

public:
	//타이머 초기화 함수
	void Init();

	//타이머 업데이트 함수(매 프레임 마다 호출)
	void Update();

};

