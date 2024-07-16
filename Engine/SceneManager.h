#pragma once

class Scene;

class SceneManager
{
private:
	SceneManager() = default;
	~SceneManager() = default;
public:
	static SceneManager& Get()
	{
		static SceneManager instnace;
		return instnace;
	}

public:
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;
private:
	shared_ptr<Scene> currentScene;
public: //public ���� ����
	shared_ptr<Scene> GetCurrentScene() { return currentScene; }

public:
	void Update();
	//Render �Լ� �߰�
	void Render();
	void LoadScene(wstring sceneName);

private:
	shared_ptr<Scene> LoadSampleScene();

};

