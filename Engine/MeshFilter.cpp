#include "pch.h"
#include "MeshFilter.h"
#include "Mesh.h"
#include "Material.h"
#include "Transform.h"	 //�߰�


void MeshFilter::Render()
{
	//�ش� Transform�� PushData ����
	GetTransform()->PushDate();

	material->Update();
	mesh->Render();

}
