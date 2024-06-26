#include "pch.h"
#include "TableDescriptor.h"
#include "Engine.h"

void TableDescriptor::Init(UINT32 count)
{
    groupCount = count;

    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.NumDescriptors = count * REGISTER_COUNT;
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; 
    desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;    

    Engine::Get().GetDevice()->GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&descHeap));

    handleSize = Engine::Get().GetDevice()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    groupSize = handleSize * REGISTER_COUNT;
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
    //SetCBV 복붙
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
    //UINT8 로 변환
    return GetCPUHandle(static_cast<UINT8>(reg));

}

D3D12_CPU_DESCRIPTOR_HANDLE TableDescriptor::GetCPUHandle(SRV_REGISTER reg)
{
    //UINT8 로 변환 GetCPUHandle의 CBV_REGISTER꺼 복사
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
