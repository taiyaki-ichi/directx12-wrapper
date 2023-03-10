#pragma once
#include<d3d12.h>
#include"dx12w_utility.hpp"

#pragma comment(lib,"d3d12.lib")

namespace dx12w
{

	class descriptor_heap
	{
		release_unique_ptr<ID3D12DescriptorHeap> ptr{};

		std::uint32_t size = 0;
		// あるハンドルから次のハンドルまでの距離のキャッシュ用
		// ディスクリプタヒープのタイプによってハンドル間の距離が異なる
		std::uint32_t increment_handle_size = 0;

	public:
		void initialize(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT size);

		// index番目のハンドルを返す
		D3D12_GPU_DESCRIPTOR_HANDLE get_GPU_handle(std::size_t index = 0);
		D3D12_CPU_DESCRIPTOR_HANDLE get_CPU_handle(std::size_t index = 0);

		ID3D12DescriptorHeap* get();
	};

	inline descriptor_heap create_descriptor_heap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT size);
	

	//
	// 以下、実装
	//


	inline void descriptor_heap::initialize(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT size)
	{
		D3D12_DESCRIPTOR_HEAP_DESC desc{};
		// レンダーターゲット用とかデプス用はシェーダから見えるフラグを立てるとエラー出る
		// TODO: サンプラーの場合はシェーダから見えた方がよくね?
		desc.Flags = type == D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		desc.NodeMask = 0;
		desc.NumDescriptors = size;
		desc.Type = type;

		// この関数内でしかdeviceに触ることができないので
		// ここで値を記録しておく
		increment_handle_size = device->GetDescriptorHandleIncrementSize(type);

		ID3D12DescriptorHeap* tmp = nullptr;
		if (FAILED(device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&tmp))))
			THROW_PDX12_EXCEPTION("");
		ptr.reset(tmp);

		this->size = size;
	}

	inline D3D12_GPU_DESCRIPTOR_HANDLE descriptor_heap::get_GPU_handle(std::size_t index)
	{
		// 基底となるハンドルからindex分インクリメントしている
		auto gpuHandle = ptr->GetGPUDescriptorHandleForHeapStart();
		gpuHandle.ptr += static_cast<UINT64>(increment_handle_size) * static_cast<UINT64>(index);
		return gpuHandle;
	}

	inline D3D12_CPU_DESCRIPTOR_HANDLE descriptor_heap::get_CPU_handle(std::size_t index)
	{
		// 基底となるハンドルからindex分インクリメントしている
		auto cpuHandle = ptr->GetCPUDescriptorHandleForHeapStart();
		cpuHandle.ptr += static_cast<SIZE_T>(increment_handle_size) * static_cast<SIZE_T>(index);
		return cpuHandle;
	}

	inline ID3D12DescriptorHeap* descriptor_heap::get()
	{
		return ptr.get();
	}

	inline descriptor_heap create_descriptor_heap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT size)
	{
		descriptor_heap result{};
		result.initialize(device, type, size);
		return result;
	}
}