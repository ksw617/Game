#pragma once
#include "Singleton.h"

class Scene;

//Singleton ���
class SceneManager : public Singleton<SceneManager>
{
	//Singleton Ŭ�󽺰� private �����ڿ� �����Ҽ� �ְ�
	friend class Singleton<SceneManager>;
private:
	SceneManager() = default;
	~SceneManager() = default;
private:
	shared_ptr<Scene> currentScene;
public: 
	shared_ptr<Scene> GetCurrentScene() { return currentScene; }

public:
	void Update();
	void Render();
	void LoadScene(wstring sceneName);

private:
	shared_ptr<Scene> LoadSampleScene();

};

