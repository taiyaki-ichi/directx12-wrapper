#pragma once
#include<d3d12.h>
#include<dxgi1_4.h>
#include"utility.hpp"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

namespace dx12w
{
	// �V�����^���`����قǂł��Ȃ��Ǝv�����̂ŃG�C���A�X�ł��܂�
	// �����݂ɂ�������
	using resource_and_state = std::pair<release_unique_ptr<ID3D12Resource>, D3D12_RESOURCE_STATES>;

	// 
	// create_commited_�֐��ł̓������m�ۂƃ��\�[�X�̍쐬���s���Ă���̂Œx��
	// CreateReservedResource, CreatePlacedResource�Ƃ�C++�ł����Ƃ���̃��v���C�X�����gnew�݂����Ȃ��Ƃ��ł���炵��
	//

	// CPU����̏������݂��\�ȃo�b�t�@
	// ���_�o�b�t�@��萔�o�b�t�@���쐬����ۂɎg�p
	resource_and_state create_commited_upload_buffer_resource(ID3D12Device* device, UINT64 size);

	// GPU����̂ݓǂݏ������\
	// �e�N�X�`���Ƃ��Ĉ������ƂōœK�����͂����, �݂����Ȋ������Ǝv��
	resource_and_state create_commited_texture_resource(ID3D12Device* device,
		DXGI_FORMAT format, UINT64 width, UINT height, std::size_t dimension, UINT16 depthOrArraySize, UINT16 mipLevels, D3D12_RESOURCE_FLAGS flags, D3D12_CLEAR_VALUE const* clearValue = nullptr);

	// ���̊֐��g���ǂ���Ȃ�����
	// create_commited_texture_resource�ō쐬�������\�[�X�̕����A�N�Z�X�����Ǝv��, ���Ԃ�
	resource_and_state create_commited_buffer_resource(ID3D12Device* device, UINT64 size, D3D12_RESOURCE_FLAGS flags);


	// ���\�[�X�o���A���쐬��resource.second���X�V
	void resource_barrior(ID3D12GraphicsCommandList* list, resource_and_state& resource, D3D12_RESOURCE_STATES afterState);


	//
	// �ȉ��A����
	//

	resource_and_state create_commited_upload_buffer_resource(ID3D12Device* device, UINT64 size)
	{
		D3D12_HEAP_PROPERTIES heapProperties{};
		heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
		heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

		D3D12_RESOURCE_DESC resourceDesc{};
		resourceDesc.Width = size;
		resourceDesc.Height = 1;
		resourceDesc.DepthOrArraySize = 1;
		resourceDesc.MipLevels = 1;
		resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
		resourceDesc.SampleDesc.Count = 1;
		resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;

		ID3D12Resource* tmp = nullptr;

		// D3D12_HEAP_TYPE_UPLOAD�̏ꍇ�̏�����Ԃ�D3D12_RESOURCE_STATE_GENERIC_READ
		D3D12_RESOURCE_STATES initialState = D3D12_RESOURCE_STATE_GENERIC_READ;

		if (FAILED(device->CreateCommittedResource(
			&heapProperties,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			initialState,
			nullptr,
			IID_PPV_ARGS(&tmp))))
		{
			THROW_PDX12_EXCEPTION("failed CreateCommittedResource");
		}

		return { release_unique_ptr<ID3D12Resource>{tmp}, initialState };
	}


	resource_and_state create_commited_texture_resource(ID3D12Device* device,
		DXGI_FORMAT format, UINT64 width, UINT height, std::size_t dimension, UINT16 depthOrArraySize, UINT16 mipLevels, D3D12_RESOURCE_FLAGS flags, D3D12_CLEAR_VALUE const* clearValue)
	{
		D3D12_HEAP_PROPERTIES heapProperties{};
		heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
		heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

		D3D12_RESOURCE_DESC resourceDesc{};
		resourceDesc.Width = width;
		resourceDesc.Height = height;
		resourceDesc.DepthOrArraySize = depthOrArraySize;
		resourceDesc.MipLevels = mipLevels;
		resourceDesc.Format = format;
		resourceDesc.SampleDesc.Count = 1;
		resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		resourceDesc.Flags = flags;

		switch (dimension)
		{
		case 1:
			resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE1D;
			break;
		case 2:
			resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
			break;
		case 3:
			resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE3D;
			break;
		default:
			break;
		}

		ID3D12Resource* tmp = nullptr;
		D3D12_RESOURCE_STATES initialState = D3D12_RESOURCE_STATE_COMMON;

		if (FAILED(device->CreateCommittedResource(
			&heapProperties,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			initialState,
			clearValue,
			IID_PPV_ARGS(&tmp))))
		{
			THROW_PDX12_EXCEPTION("failed CreateCommittedResource");
		}

		return { release_unique_ptr<ID3D12Resource>{tmp},initialState };
	}

	resource_and_state create_commited_buffer_resource(ID3D12Device* device, UINT64 size, D3D12_RESOURCE_FLAGS flags)
	{
		D3D12_HEAP_PROPERTIES heapProperties{};
		heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
		heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

		D3D12_RESOURCE_DESC resourceDesc{};
		resourceDesc.Width = size;
		resourceDesc.Height = 1;
		resourceDesc.DepthOrArraySize = 1;
		resourceDesc.MipLevels = 1;
		resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
		resourceDesc.SampleDesc.Count = 1;
		resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resourceDesc.Flags = flags;

		ID3D12Resource* tmp = nullptr;

		D3D12_RESOURCE_STATES initialState = D3D12_RESOURCE_STATE_COMMON;

		if (FAILED(device->CreateCommittedResource(
			&heapProperties,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			initialState,
			nullptr,
			IID_PPV_ARGS(&tmp))))
		{
			THROW_PDX12_EXCEPTION("failed CreateCommittedResource");
		}

		return { release_unique_ptr<ID3D12Resource>{tmp}, initialState };
	}

	// ���\�[�X�o���A���쐬��resource.second���X�V
	// TODO: ���݂�D3D12_RESOURCE_STATES��afterState�������ȏꍇ�G���[�o���C������
	void resource_barrior(ID3D12GraphicsCommandList* list, resource_and_state& resource, D3D12_RESOURCE_STATES afterState)
	{
		D3D12_RESOURCE_BARRIER barrier{};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = resource.first.get();
		barrier.Transition.StateBefore = resource.second;
		barrier.Transition.StateAfter = afterState;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

		list->ResourceBarrier(1, &barrier);

		resource.second = afterState;
	}
}