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

    //0->1
    Engine::Get().GetCmdQueue()->GetCmdList()->SetGraphicsRootDescriptorTable(1, handle);

    currentGroupIndex++;
}

D3D12_CPU_DESCRIPTOR_HANDLE TableDescriptor::GetCPUHandle(CBV_REGISTER reg)
{
    return GetCPUHandle(static_cast<UINT8>(reg));
}

D3D12_CPU_DESCRIPTOR_HANDLE TableDescriptor::GetCPUHandle(SRV_REGISTER reg)
{
    return GetCPUHandle(static_cast<UINT8>(reg));
}

D3D12_CPU_DESCRIPTOR_HANDLE TableDescriptor::GetCPUHandle(UINT8 reg)
{
    //register는 0초과
    assert(reg > 0);
    D3D12_CPU_DESCRIPTOR_HANDLE handle = descHeap->GetCPUDescriptorHandleForHeapStart();

    handle.ptr += currentGroupIndex * groupSize;
    //전체 register에서 1빼주기
    handle.ptr += (reg - 1) * handleSize;

    return handle;
}
