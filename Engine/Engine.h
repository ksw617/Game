#pragma once
class Engine
{
	// 기본 생성자 소멸자를 private으로 선언하여
	// 외부에서 직접 생성/소멸 불가 
private:   
	//생성자 소멸자 default
	Engine() = default; 
	~Engine() = default;
public:
	//싱글톤
	static Engine& Get()
	{
		static Engine instnace;
		return instnace;
	}

public:
	//복사 방지
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;

private:
	HWND hwnd; //출력할 윈도우 핸들
	int width; //화면 너비
	int height;//화면 높이
	bool windowed; //창 모드 여부(true : 창모드, false : 전체화면)

	D3D12_VIEWPORT viewPort = {}; //뷰포트 설정
	D3D12_RECT scissorRect = {}; //시저(Rect) 설정

	shared_ptr<class Device> device; // Device 클라스의 스마트 포인터
	shared_ptr<class CommandQueue> cmdQueue; // CommandQueue 클라스의 스마트 포인터
	shared_ptr<class SwapChain> swapChain; // SwapChain 클라의 스마트 포인터
public:
	//엔진 초기화 함수
	void Init(HWND _hwnd, int _width, int _height, bool _windowed);
	//랜더링 함수
	void Render();

public:
	//랜더링 시작 함수
	void RederBegin();
	//랜더링 종료 함수
	void RenderEnd();

	//윈도우 크기 조정 함수
	void ResizeWindow(int _width, int _height);
};

