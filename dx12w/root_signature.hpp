#pragma once
#include<d3d12.h>
#include"utility.hpp"
#include<vector>
#include<array>


namespace pdx12
{
	// ���[�g�V�O�l�`���̏������p���炢�����g��Ȃ�
	// D3D12_STATIC_SAMPLER_DESC�Ƃ���ܕς�Ȃ�����Ȃ����Ă��C�C����
	struct static_sampler
	{
		D3D12_FILTER filter;
		D3D12_TEXTURE_ADDRESS_MODE address_u;
		D3D12_TEXTURE_ADDRESS_MODE address_v;
		D3D12_TEXTURE_ADDRESS_MODE address_w;
		D3D12_COMPARISON_FUNC comparison_func;


		UINT max_anisotropy = 16;
		float min_LOD = 0.f;
		float max_LOD = D3D12_FLOAT32_MAX;
		float mip_LOD_bias = 0;
	};

	// �A�����������^�C�v�̃f�B�X�N���v�^�����W��\������p
	struct descriptor_range_type
	{
		D3D12_DESCRIPTOR_RANGE_TYPE descriptorRangeType;
		UINT num = 1;
	};
	
	// �f�B�X�N���v�^�����W�ƃX�^�e�B�b�N�T���v�����ăR���p�C�����ɂ����Ă����܂��Ă���
	// vector�g���̂͂ǂ��Ȃ񂾂�
	// �f�B�X�N���v�^�����W�̃��W�X�^�̔ԍ��͏��Ɏ����Ŋ����Ă���
	release_unique_ptr<ID3D12RootSignature> create_root_signature(ID3D12Device* device,
		std::vector<std::vector<descriptor_range_type>> const& descriptorRangeTypes, std::vector<static_sampler> const& staticSamplers);
}