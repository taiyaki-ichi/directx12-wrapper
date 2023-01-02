#pragma once
#include<d3d12.h>
#include<dxgi1_4.h>
#include"dx12w_utility.hpp"
#include<array>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

namespace dx12w
{

	// �R�}���h�A���P�[�^�̐��̓R���p�C�����Ɍ��܂��Ă�C������
	template<std::size_t AllocatorNum>
	class command_manager
	{
		release_unique_ptr<ID3D12CommandQueue> queue{};

		release_unique_ptr<ID3D12GraphicsCommandList> list{};

		// �񓯊��ɏ�������ꍇ, CommandQueue��CommandList��1�ŃC�C��Allocator�͕����쐬���Ȃ��ƃ_��
		std::array<release_unique_ptr<ID3D12CommandAllocator>, AllocatorNum> allocators{};

		// allocators�̃C���f�b�N�X�Ɠ������p�ӂ�, �C�ӂ̃^�C�~���O�ő҂��Ƃ��ł���悤�ɂ���
		std::array<release_unique_ptr<ID3D12Fence>, AllocatorNum> fences{};
		std::array<std::uint64_t, AllocatorNum> fence_values{};

		// �t�F���X�̒l���w�肵���l�ɂȂ�܂ő҂C�x���g�������p
		HANDLE fence_event_handle = nullptr;

		// ���݂�list���g�p���Ă���allocaotr�̃C���f�b�N�X
		std::size_t current_allocaotr_index = 0;

	public:
		// ������
		void initialize(ID3D12Device* device);

		// �t�F���X�𗧂Ă�
		void signal();

		// ���Ă��t�F���X��҂�
		void wait(std::size_t index);

		// index�Ԗڂ�Allocator�Ń��X�g�����Z�b�g
		// �ȍ~�ς܂ꂽ�R�}���h��index�Ԗڂ̃A���P�[�^�ɐς܂�邱�ƂɂȂ�
		void reset_list(std::size_t index);



		void excute();

		// excute�����邩��dispatch�������ɍ���Ă���
		void dispatch(std::uint32_t threadGroupCountX, std::uint32_t threadGroupCountY, std::uint32_t threadGroupCountZ);



		ID3D12GraphicsCommandList* get_list();

		ID3D12CommandQueue* get_queue();
	};

	template<std::size_t AllocatorNum>
	command_manager<AllocatorNum> create_command_manager(ID3D12Device* device);

	// 
	// 
	// 

	template<std::size_t AllocatorNum>
	inline void command_manager<AllocatorNum>::initialize(ID3D12Device* device)
	{
		// allocator�̍쐬
		for (std::size_t i = 0; i < AllocatorNum; i++)
		{
			ID3D12CommandAllocator* tmp = nullptr;
			if (FAILED(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&tmp))))
				THROW_PDX12_EXCEPTION("");
			allocators[i].reset(tmp);
		}

		// ������Ԃł�0�Ԗڂ��g�p
		current_allocaotr_index = 0;

		// list�̍쐬
		{
			ID3D12GraphicsCommandList* tmp = nullptr;
			if (FAILED(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, allocators[current_allocaotr_index].get(), nullptr, IID_PPV_ARGS(&tmp))))
				THROW_PDX12_EXCEPTION("");
			list.reset(tmp);
		}

		// queue�̍쐬
		{
			ID3D12CommandQueue* tmp = nullptr;
			D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};
			cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;		// �^�C���A�E�g�i�V
			cmdQueueDesc.NodeMask = 0;
			cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;	// �v���C�I���e�B���Ɏw��Ȃ�
			cmdQueueDesc.Type = list->GetType();			// �����̓R�}���h���X�g�ƍ��킹��
			if (FAILED(device->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&tmp))))
				THROW_PDX12_EXCEPTION("");
			queue.reset(tmp);
		}

		// 0�ŏ�����
		std::fill(fence_values.begin(), fence_values.end(), 0);

		// fence�쐬
		for (std::size_t i = 0; i < AllocatorNum; i++)
		{
			ID3D12Fence* tmp = nullptr;
			if (FAILED(device->CreateFence(fence_values[i], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&tmp))))
				THROW_PDX12_EXCEPTION("");
			fences[i].reset(tmp);
		}

		fence_event_handle = CreateEvent(NULL, FALSE, FALSE, NULL);

		// �쐬����list��close����Ă��Ȃ��̂ŌĂяo���K�v������
		list->Close();
	}


	template<std::size_t AllocatorNum>
	inline void dx12w::command_manager<AllocatorNum>::signal()
	{
		// �C���N�������g���ĈȑO�Ɏg�p�����l��ύX����
		fence_values[current_allocaotr_index]++;
		// �ݒ肳��Ă���A���P�[�^�̃C���f�b�N�X�ɑΉ������t�F���X���g�p����
		queue->Signal(fences[current_allocaotr_index].get(), fence_values[current_allocaotr_index]);
	}

	template<std::size_t AllocatorNum>
	inline void dx12w::command_manager<AllocatorNum>::wait(std::size_t index)
	{
		if (fences[index]->GetCompletedValue() < fence_values[index])
		{
			// fence�̒l���w�肵���l�ɂȂ��Ă��Ȃ�
			// �܂�V�O�i���𗧂Ă��ꏊ�܂ł̏������I����Ă��Ȃ��ꍇ
			// �t�F���X�̒l���w�肵���l�ɂȂ������ɔ��΂���C�x���g��ݒ肷��
			fences[index]->SetEventOnCompletion(fence_values[index], fence_event_handle);
			// �C�x���g�����΂���̂�҂�
			WaitForSingleObject(fence_event_handle, INFINITE);
		}
	}

	template<std::size_t AllocatorNum>
	inline void dx12w::command_manager<AllocatorNum>::reset_list(std::size_t index)
	{
		allocators[index]->Reset();
		list->Reset(allocators[index].get(), nullptr);
		current_allocaotr_index = index;
	}

	template<std::size_t AllocatorNum>
	inline void dx12w::command_manager<AllocatorNum>::excute()
	{
		queue->ExecuteCommandLists(1, (ID3D12CommandList**)(&list));
	}

	template<std::size_t AllocatorNum>
	inline void dx12w::command_manager<AllocatorNum>::dispatch(std::uint32_t threadGroupCountX, std::uint32_t threadGroupCountY, std::uint32_t threadGroupCountZ)
	{
		list->Dispatch(threadGroupCountX, threadGroupCountY, threadGroupCountZ);
	}

	template<std::size_t AllocatorNum>
	inline ID3D12GraphicsCommandList* dx12w::command_manager<AllocatorNum>::get_list()
	{
		return list.get();
	}

	template<std::size_t AllocatorNum>
	inline ID3D12CommandQueue* dx12w::command_manager<AllocatorNum>::get_queue()
	{
		return queue.get();
	}

	template<std::size_t AllocatorNum>
	command_manager<AllocatorNum> create_command_manager(ID3D12Device* device)
	{
		command_manager<AllocatorNum> result{};
		result.initialize(device);
		return result;
	}
}