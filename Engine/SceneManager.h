#pragma once
#include "Singleton.h"

class Scene;

//레이어의 최대 갯수를 정의
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

	//레이어 이름을 저장하는 배열
	array<wstring, MAX_LAYER> layerNames;

	//레이어 이름과 인덱스를 매핑하는 map
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
	//레이어 이름을 설정하는 함수
	void SetLayerName(UINT8 index, const wstring& name);

	//인덱스를 레이어 이름으로 변환하는 함수
	const wstring& IndexToLayerName(UINT8 index) { return layerNames[index]; }

	//레이어 이름을 인덱스로 변환하는 함수
	UINT8 LayerNameToIndex(const wstring& name);
};

