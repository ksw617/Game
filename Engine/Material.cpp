#include "pch.h"
#include "Material.h"
#include "Engine.h"

void Material::Update()
{
	//CBV 업로드
	//Material 매개변수를 ConstantBuffer에 푸시 
	Engine::Get().GetConstantBuffer(CONSTANT_BUFFER_TYPE::MATERIAL)->PushData(&params, sizeof(params));

	//모든 텍스처를 순회하며 쉐이더 리소스 뷰(SRV)를 설정
	for (UINT32 i = 0; i < textures.size(); i++)
	{
		//텍스처가 nullptr이면
		if (textures[i] == nullptr)
			continue;  //건너뜀

		//SRV 레지스터를 계산하여 설정
		SRV_REGISTER reg = SRV_REGISTER(static_cast<INT8>(SRV_REGISTER::t0) + i);
		Engine::Get().GetTableDesc()->SetSRV(textures[i]->GetCpuHandle(), reg);

	}

	//쉐이더 업데이트
	shader->Update();
}
