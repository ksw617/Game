#include "pch.h"
#include "MeshFilter.h"
#include "Mesh.h"
#include "Material.h"


void MeshFilter::Render()
{
	//Material ������Ʈ
	material->Update();

	//Mesh ����
	mesh->Render();

}
