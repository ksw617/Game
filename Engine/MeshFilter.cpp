#include "pch.h"
#include "MeshFilter.h"
#include "Mesh.h"
#include "Material.h"
#include "Transform.h"	 //추가


void MeshFilter::Render()
{
	//해당 Transform의 PushData 실행
	GetTransform()->PushDate();

	material->Update();
	mesh->Render();

}
