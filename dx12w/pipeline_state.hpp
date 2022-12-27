#pragma once
#include<d3d12.h>
#include<dxgi1_4.h>
#include"utility.hpp"
#include<vector>


namespace pdx12
{
	// �O���t�B�b�N�p�C�v���C���̏������p
	struct input_element
	{
		char const* name;
		DXGI_FORMAT format;
	};

	// �O���t�B�b�N�p�C�v���C���̏������p
	// �[�x���Ƃ邾���Ƃ���renderTargerFormats����̏ꍇ��shader_desc.pixel_shader��nullpter�ŗǂ������C������
	// �e�b�Z���[�V�����������ꍇshader_desc.hull_shader��shader_desc.domain_shader�̂ǂ�����ݒ肷��K�v����
	// ��{��shader_desc.vertex_shader��shader_desc.pixel_shader�ɒl���ݒ肳��Ă�΂���
	struct shader_desc 
	{
		ID3DBlob* vertex_shader = nullptr;
		ID3DBlob* pixcel_shader = nullptr;
		ID3DBlob* geometry_shader = nullptr;
		ID3DBlob* hull_shader = nullptr;
		ID3DBlob* domain_shader = nullptr;
	};

	// �������傢���낢��ݒ�ł���悤�ɕύX������
	// ���C���[�t���[���ŕ`�ʂł�����, �X�e���V���e�X�g�ł�����Ƃ�
	release_unique_ptr<ID3D12PipelineState> create_graphics_pipeline(ID3D12Device* device,
		ID3D12RootSignature* rootSignature, std::vector<input_element> const& inputElements, std::vector<DXGI_FORMAT> const& renderTargetFormats,
		shader_desc const& shaderDescs, bool depthEnable, bool alphaBlend, D3D12_PRIMITIVE_TOPOLOGY_TYPE primitiveTopologyType);

	release_unique_ptr<ID3D12PipelineState> create_compute_pipeline(ID3D12Device* device,
		ID3D12RootSignature* rootSignature, ID3DBlob* computeShader);
}