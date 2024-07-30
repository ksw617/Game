#pragma once
#include "Singleton.h"

class Scene;

//���̾��� �ִ� ������ ����
enum 
{
	 MAX_LAYER = 32
};

class SceneManager : public Singleton<SceneManager>
{
	friend class Singleton<SceneManager>;
private:
	SceneManager() = default;
	~SceneManager() = default;
private:
	shared_ptr<Scene> currentScene;

	//���̾� �̸��� �����ϴ� �迭
	array<wstring, MAX_LAYER> layerNames;

	//���̾� �̸��� �ε����� �����ϴ� map
	map<wstring, UINT8> layerIndex;
public: 
	shared_ptr<Scene> GetCurrentScene() { return currentScene; }

public:
	void Update();
	void Render();
	void LoadScene(wstring sceneName);

private:
	shared_ptr<Scene> LoadSampleScene();

public:
	//���̾� �̸��� �����ϴ� �Լ�
	void SetLayerName(UINT8 index, const wstring& name);

	//�ε����� ���̾� �̸����� ��ȯ�ϴ� �Լ�
	const wstring& IndexToLayerName(UINT8 index) { return layerNames[index]; }

	//���̾� �̸��� �ε����� ��ȯ�ϴ� �Լ�
	UINT8 LayerNameToIndex(const wstring& name);
};

