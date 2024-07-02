#pragma once

class GameObject;

class Scene
{
private:
	//씬 내의 GameObject들을 저장하는 vector
	vector<shared_ptr<GameObject>> gameObjects;

public:
	//로직 함수
	void Awake();
	void Start();
	void Update();
	void LateUpdate();

public:
	//GameObject 추가
	void AddGameObject(shared_ptr<GameObject> gameObject);
	//GameObject 제거
	void RemoveGameObject(shared_ptr<GameObject> gameObject);
};

