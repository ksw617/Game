#pragma once
#include "Singleton.h"

class Scene;

//Singleton 상속
class SceneManager : public Singleton<SceneManager>
{
	//Singleton 클라스가 private 생성자에 접근할수 있게
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

