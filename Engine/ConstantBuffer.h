#pragma once
class ConstantBuffer
{
private:
	ComPtr<ID3D12Resource> cbvBuffer;
	BYTE* mappedBuffer = nullptr;
	int elementSize = 0;
	int elementCount = 0;
	int currentIndex = 0;

public:
	ConstantBuffer();
	~ConstantBuffer();
private:
	//��� ���� ����
	void CreateBuffer();
public:
	//��� ���� �ʱ�ȭ
	void Init(int size, int count);
	//��� ���� ������ �ʱ�ȭ
	void Clear();
	//��� ���ۿ� ������ Ǫ��
	void PushData(int rootParamIndex, void* buffer, int size);

	//GPU ���� �ּ� ��������
	D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress(int index);

};

