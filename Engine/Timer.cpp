#include "pch.h"
#include "Timer.h"

void Timer::Init()
{
	//타이머 주파수를 얻어와 frequency 변수에 저장
	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&frequency));
	//현재 카운트 값을 얻어와 prevCount 변수에 저장 (초기화)
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&prevCount)); // CPU 클럭
}

void Timer::Update()
{
	UINT64 currentCount;

	//현재 카운트 값을 얻어와 currentCount 변수에 저장
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currentCount));

	//두 프레임 간의 시간 간격(deltaTime)을 계산
	deltaTime = (currentCount - prevCount) / static_cast<float>(frequency);

	//현재 카운트 값을 prevCount에 저장하여 다음 프레임에 사용할 수 있게 함
	prevCount = currentCount;

	//프레임 카운트를 증가
	frameCount++;

	//프레임 시간 누적값에 deltaTime을 더함
	frameTime += deltaTime;

	//누적된 프레임 시간이 1초를 초과하면 FPS 계산
	if (frameTime > 1.f)
	{
		//FPS 계산: frameCount를 frameTime으로 나누어 초당 프레임 수를 구함
		fps = static_cast<UINT32>(frameCount / frameTime);

		//프레임 시간과 프레임 카운트를 초기화
		frameTime = 0.f;
		frameCount = 0;
	}
}
