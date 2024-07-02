#include "pch.h"
#include "Material.h"
#include "Engine.h"

void Material::Update()
{
	//CBV ���ε�
	//Material �Ű������� ConstantBuffer�� Ǫ�� 
	Engine::Get().GetConstantBuffer(CONSTANT_BUFFER_TYPE::MATERIAL)->PushData(&params, sizeof(params));

	//��� �ؽ�ó�� ��ȸ�ϸ� ���̴� ���ҽ� ��(SRV)�� ����
	for (UINT32 i = 0; i < textures.size(); i++)
	{
		//�ؽ�ó�� nullptr�̸�
		if (textures[i] == nullptr)
			continue;  //�ǳʶ�

		//SRV �������͸� ����Ͽ� ����
		SRV_REGISTER reg = SRV_REGISTER(static_cast<INT8>(SRV_REGISTER::t0) + i);
		Engine::Get().GetTableDesc()->SetSRV(textures[i]->GetCpuHandle(), reg);

	}

	//���̴� ������Ʈ
	shader->Update();
}