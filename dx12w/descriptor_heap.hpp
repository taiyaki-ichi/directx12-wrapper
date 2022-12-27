#pragma once
#include<d3d12.h>
#include"utility.hpp"

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
}