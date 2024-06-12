#pragma once
class Engine
{
	// �⺻ ������ �Ҹ��ڸ� private���� �����Ͽ�
	// �ܺο��� ���� ����/�Ҹ� �Ұ� 
private:   
	//������ �Ҹ��� default
	Engine() = default; 
	~Engine() = default;
public:
	//�̱���
	static Engine& Get()
	{
		static Engine instnace;
		return instnace;
	}

public:
	//���� ����
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;

private:
	HWND hwnd; //����� ������ �ڵ�
	int width; //ȭ�� �ʺ�
	int height;//ȭ�� ����
	bool windowed; //â ��� ����(true : â���, false : ��üȭ��)

	D3D12_VIEWPORT viewPort = {}; //����Ʈ ����
	D3D12_RECT scissorRect = {}; //����(Rect) ����

	shared_ptr<class Device> device; // Device Ŭ���� ����Ʈ ������
	shared_ptr<class CommandQueue> cmdQueue; // CommandQueue Ŭ���� ����Ʈ ������
	shared_ptr<class SwapChain> swapChain; // SwapChain Ŭ���� ����Ʈ ������
public:
	//���� �ʱ�ȭ �Լ�
	void Init(HWND _hwnd, int _width, int _height, bool _windowed);
	//������ �Լ�
	void Render();

public:
	//������ ���� �Լ�
	void RederBegin();
	//������ ���� �Լ�
	void RenderEnd();

	//������ ũ�� ���� �Լ�
	void ResizeWindow(int _width, int _height);
};

