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
public:
	//Scene에 있는 모든 GameObject들에게 접근하기 위해
	const vector<shared_ptr<GameObject>>& GetGameObjects() { return gameObjects; }

public:
	void AddGameObject(shared_ptr<GameObject> gameObject);
	void RemoveGameObject(shared_ptr<GameObject> gameObject);
};

