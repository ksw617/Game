#pragma once
#include "Component.h"

class Mesh;
class Material;

//Component 상속
class MeshFilter : public Component
{
private:
	//Mesh 데이터를 가리키는 스마트 포인터
	shared_ptr<Mesh> mesh;
	//Material 데이터를 가리키는 스마트 포인터
	shared_ptr<Material> material;

public:
	//생성자
	//컴포넌트 타입을 MESH_FILTER으로 설정하여 Component 생성자 호출
	MeshFilter() : Component(COMPONENT_TYPE::MESH_FILTER) {}
	//소멸자
	virtual ~MeshFilter() {}

public:
	//Mesh set 함수
	void SetMesh(shared_ptr<Mesh> _mesh) { mesh = _mesh; }
	//Material set 함수
	void SetMaterial(shared_ptr<Material> _mat) { material = _mat; }
public:
	//랜더링 함수
	void Render();
	//매 프레임 업데이트 할때 마다 {랜더링 함수 실행}
	virtual void Update() override { Render(); }

};

