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

void TableDescriptor::CommitTable()
{
    //디스크립터 힙의 시작 GPU 디스크립터 핸들을 가져옴
    D3D12_GPU_DESCRIPTOR_HANDLE handle = descHeap->GetGPUDescriptorHandleForHeapStart();

    //현재 그룹 인덱스를 기준으로 핸들을 오프셋
    handle.ptr += currentGroupIndex * groupSize;

    //CmdList에 루트 디스크립터 테이블 설정
    Engine::Get().GetCmdQueue()->GetCmdList()->SetGraphicsRootDescriptorTable(0, handle);

    //다음 그룹 인덱스로 증가
    currentGroupIndex++;
}

D3D12_CPU_DESCRIPTOR_HANDLE TableDescriptor::GetCPUHandle(CBV_REGISTER reg)
{
    return GetCPUHandle(static_cast<UINT32>(reg));

}

D3D12_CPU_DESCRIPTOR_HANDLE TableDescriptor::GetCPUHandle(UINT32 reg)
{
    D3D12_CPU_DESCRIPTOR_HANDLE handle = descHeap->GetCPUDescriptorHandleForHeapStart();

    handle.ptr += currentGroupIndex * groupSize;
    handle.ptr += reg * handleSize;

    return handle;
}
