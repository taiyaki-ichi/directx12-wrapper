#pragma once
#include<d3d12.h>
#include<dxgi1_4.h>
#include"utility.hpp"

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
}