#pragma once

class GameObject;

class Scene
{
private:
	vector<shared_ptr<GameObject>> gameObjects;
public:
	void Awake();
	void Start();
	void Update();
	void LateUpdate();
	void FinalUpdate();

	//씬 자체에 Render 함수
	void Render();
private:
	//라이트 데이터들 밀어넣기
	void PushLightData();
public:	
	const vector<shared_ptr<GameObject>>& GetGameObjects() { return gameObjects; }

public:
	void AddGameObject(shared_ptr<GameObject> gameObject);
	void RemoveGameObject(shared_ptr<GameObject> gameObject);
};

