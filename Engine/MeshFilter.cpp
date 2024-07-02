#include "pch.h"
#include "MeshFilter.h"
#include "Mesh.h"
#include "Material.h"


void MeshFilter::Render()
{
	//Material 업데이트
	material->Update();

	//Mesh 랜더
	mesh->Render();

}
