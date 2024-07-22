#include "pch.h"
#include "TableDescriptor.h"
#include "Engine.h"

void TableDescriptor::Init(UINT32 count)
{
    groupCount = count;

    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.NumDescriptors = count * (REGISTER_COUNT - 1); //b0~b4 => b1~b4 사용
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; 
    desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;    

    Engine::Get().GetDevice()->GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&descHeap));

    handleSize = Engine::Get().GetDevice()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    groupSize = handleSize * (REGISTER_COUNT - 1); //b0~b4 => b1~b4 사용
}

void TableDescriptor::Clear()
{
    currentGroupIndex = 0;
}

void TableDescriptor::SetCBV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, CBV_REGISTER reg)
{
    D3D12_CPU_DESCRIPTOR_HANDLE destHandle = GetCPUHandle(reg);

    UINT32 destRange = 1;     
    UINT32 srcRange = 1;       

    Engine::Get().GetDevice()->GetDevice()->CopyDescriptors(
        1, &destHandle, &destRange,
        1, &srcHandle, &srcRange, 
        D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV 
    );
}

void TableDescriptor::SetSRV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, SRV_REGISTER reg)
{
    D3D12_CPU_DESCRIPTOR_HANDLE destHandle = GetCPUHandle(reg);

    UINT32 destRange = 1;
    UINT32 srcRange = 1;

    Engine::Get().GetDevice()->GetDevice()->CopyDescriptors(
        1, &destHandle, &destRange,
        1, &srcHandle, &srcRange,
        D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
    );
}

void TableDescriptor::CommitTable()
{
    D3D12_GPU_DESCRIPTOR_HANDLE handle = descHeap->GetGPUDescriptorHandleForHeapStart();
    handle.ptr += currentGroupIndex * groupSize;

    Engine::Get().GetCmdQueue()->GetCmdList()->SetGraphicsRootDescriptorTable(0, handle);

    currentGroupIndex++;
}

D3D12_CPU_DESCRIPTOR_HANDLE TableDescriptor::GetCPUHandle(CBV_REGISTER reg)
{
    //CBV_REGISTER는 b1부터 사용하게 하여 올바른 핸들을 얻음
    UINT8 cbv_reg = static_cast<UINT8>(reg);
    assert(cbv_reg > 0); //cbv_reg가 0보다 큰지 체크

    //b0은 전역으로 사용할꺼니까 Reg를 1빼줌 b1~b4사용 
    return GetCPUHandle(cbv_reg - 1);

}

D3D12_CPU_DESCRIPTOR_HANDLE TableDescriptor::GetCPUHandle(SRV_REGISTER reg)
{
    return GetCPUHandle(static_cast<UINT8>(reg));
}

//UINT8용으로 바꿈
D3D12_CPU_DESCRIPTOR_HANDLE TableDescriptor::GetCPUHandle(UINT8 reg)
{
    D3D12_CPU_DESCRIPTOR_HANDLE handle = descHeap->GetCPUDescriptorHandleForHeapStart();

    handle.ptr += currentGroupIndex * groupSize;
    handle.ptr += reg * handleSize;

    return handle;
}
