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
		// ����n���h�����玟�̃n���h���܂ł̋����̃L���b�V���p
		// �f�B�X�N���v�^�q�[�v�̃^�C�v�ɂ���ăn���h���Ԃ̋������قȂ�
		std::uint32_t increment_handle_size = 0;

	public:
		void initialize(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT size);

		// index�Ԗڂ̃n���h����Ԃ�
		D3D12_GPU_DESCRIPTOR_HANDLE get_GPU_handle(std::size_t index = 0);
		D3D12_CPU_DESCRIPTOR_HANDLE get_CPU_handle(std::size_t index = 0);

		ID3D12DescriptorHeap* get();
	};
}