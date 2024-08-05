#pragma once
#include "Component.h"

class Mesh;
class Material;

class MeshFilter : public Component
{
private:
	shared_ptr<Mesh> mesh;
	shared_ptr<Material> material;

public:
	MeshFilter() : Component(COMPONENT_TYPE::MESH_FILTER) {}
	virtual ~MeshFilter() {}

public:
	//material Get ÇÔ¼ö
	shared_ptr<Material> GetMaterial() { return material; }
public:
	void SetMesh(shared_ptr<Mesh> _mesh) { mesh = _mesh; }
	void SetMaterial(shared_ptr<Material> _mat) { material = _mat; }
public:
	void Render();

};

