#pragma once
#include<d3d12.h>
#include<dxgi1_4.h>
#include"dx12w_utility.hpp"
#include<vector>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

namespace dx12w
{
	// �O���t�B�b�N�p�C�v���C���̏������p
	struct input_element
	{
		char const* name;
		DXGI_FORMAT format;
	};

	struct shader
	{
		void* data = nullptr;
		std::size_t size = 0;
	};

	// �O���t�B�b�N�p�C�v���C���̏������p
	// �[�x���Ƃ邾���Ƃ���renderTargerFormats����̏ꍇ��shader_desc.pixel_shader��nullpter�ŗǂ������C������
	// �e�b�Z���[�V�����������ꍇshader_desc.hull_shader��shader_desc.domain_shader�̂ǂ�����ݒ肷��K�v����
	// ��{��shader_desc.vertex_shader��shader_desc.pixel_shader�ɒl���ݒ肳��Ă�΂���
	struct shader_desc
	{
		shader* vertex_shader = nullptr;
		shader* pixcel_shader = nullptr;
		shader* geometry_shader = nullptr;
		shader* hull_shader = nullptr;
		shader* domain_shader = nullptr;
	};

	// �O���t�B�N�X�p�C�v���C���̍쐬
	// �������傢���낢��ݒ�ł���悤�ɕύX������
	// ���C���[�t���[���ŕ`�ʂł�����, �X�e���V���e�X�g�ł�����Ƃ�
	inline release_unique_ptr<ID3D12PipelineState> create_graphics_pipeline(ID3D12Device* device,
		ID3D12RootSignature* rootSignature, std::vector<input_element> const& inputElements, std::vector<DXGI_FORMAT> const& renderTargetFormats,
		shader_desc const& shaderDescs, bool depthEnable, bool alphaBlend, D3D12_PRIMITIVE_TOPOLOGY_TYPE primitiveTopologyType);

	// �R���s���[�g�V�F�[�_���g���ۂ̃p�C�|���C���̍쐬
	inline release_unique_ptr<ID3D12PipelineState> create_compute_pipeline(ID3D12Device* device,
		ID3D12RootSignature* rootSignature, ID3DBlob* computeShader);


	//
	// �ȉ��A����
	//

	inline release_unique_ptr<ID3D12PipelineState> create_graphics_pipeline(ID3D12Device* device,
		ID3D12RootSignature* rootSignature, std::vector<input_element> const& inputElements, std::vector<DXGI_FORMAT> const& renderTargetFormats,
		shader_desc const& shaderDescs, bool depthEnable, bool alphaBlend, D3D12_PRIMITIVE_TOPOLOGY_TYPE primitiveTopologyType)
	{
		D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineDesc{};

		graphicsPipelineDesc.PrimitiveTopologyType = primitiveTopologyType;
		graphicsPipelineDesc.pRootSignature = rootSignature;

		//
		// �V�F�[�_�̐ݒ�
		//
		if (shaderDescs.vertex_shader) {
			graphicsPipelineDesc.VS.pShaderBytecode = shaderDescs.vertex_shader->data;
			graphicsPipelineDesc.VS.BytecodeLength = shaderDescs.vertex_shader->size;
		}
		if (shaderDescs.pixcel_shader) {
			graphicsPipelineDesc.PS.pShaderBytecode = shaderDescs.pixcel_shader->data;
			graphicsPipelineDesc.PS.BytecodeLength = shaderDescs.pixcel_shader->size;
		}
		if (shaderDescs.geometry_shader) {
			graphicsPipelineDesc.GS.pShaderBytecode = shaderDescs.geometry_shader->data;
			graphicsPipelineDesc.GS.BytecodeLength = shaderDescs.geometry_shader->size;
		}
		if (shaderDescs.hull_shader) {
			graphicsPipelineDesc.HS.pShaderBytecode = shaderDescs.hull_shader->data;
			graphicsPipelineDesc.HS.BytecodeLength = shaderDescs.hull_shader->size;
		}
		if (shaderDescs.domain_shader) {
			graphicsPipelineDesc.DS.pShaderBytecode = shaderDescs.domain_shader->data;
			graphicsPipelineDesc.DS.BytecodeLength = shaderDescs.domain_shader->size;
		}


		//
		// ���͏��̐ݒ�
		//
		std::vector<D3D12_INPUT_ELEMENT_DESC> inputElementDescs(inputElements.size());
		for (std::size_t i = 0; i < inputElements.size(); i++)
		{
			inputElementDescs[i].SemanticName = inputElements[i].name;
			inputElementDescs[i].Format = inputElements[i].format;
			inputElementDescs[i].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
			inputElementDescs[i].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
		}

		graphicsPipelineDesc.InputLayout.pInputElementDescs = inputElementDescs.data();
		graphicsPipelineDesc.InputLayout.NumElements = static_cast<UINT>(inputElementDescs.size());


		//
		// ���X�^���C�U�̐ݒ�
		//
		D3D12_RASTERIZER_DESC rasterizerDesc{};
		rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
		rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
		rasterizerDesc.FrontCounterClockwise = FALSE;
		rasterizerDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
		rasterizerDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
		rasterizerDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
		rasterizerDesc.DepthClipEnable = TRUE;
		rasterizerDesc.MultisampleEnable = FALSE;
		rasterizerDesc.AntialiasedLineEnable = FALSE;
		rasterizerDesc.ForcedSampleCount = 0;
		rasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

		graphicsPipelineDesc.RasterizerState = rasterizerDesc;


		//
		// �u�����h�X�e�[�g�̐ݒ�
		//
		D3D12_RENDER_TARGET_BLEND_DESC renderTagetBlendDesc{};
		if (alphaBlend)
		{
			renderTagetBlendDesc.BlendEnable = TRUE;
			renderTagetBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
			renderTagetBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
			renderTagetBlendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;;
			renderTagetBlendDesc.DestBlendAlpha = D3D12_BLEND_ONE;
			renderTagetBlendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
			renderTagetBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
			renderTagetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			renderTagetBlendDesc.LogicOp = D3D12_LOGIC_OP_NOOP;
			renderTagetBlendDesc.LogicOpEnable = FALSE;
		}
		else {
			renderTagetBlendDesc.BlendEnable = FALSE;
			renderTagetBlendDesc.LogicOpEnable = FALSE;
			renderTagetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		}

		D3D12_BLEND_DESC blendDesc{};
		blendDesc.AlphaToCoverageEnable = TRUE;
		blendDesc.IndependentBlendEnable = FALSE;
		blendDesc.RenderTarget[0] = renderTagetBlendDesc;

		graphicsPipelineDesc.BlendState = blendDesc;


		//
		// �����_�[�^�[�Q�b�g�̐ݒ�
		//
		graphicsPipelineDesc.NumRenderTargets = static_cast<UINT>(renderTargetFormats.size());
		std::copy(renderTargetFormats.begin(), renderTargetFormats.end(), std::begin(graphicsPipelineDesc.RTVFormats));


		//
		// �f�v�X�X�e���V���̐ݒ�
		//
		D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
		depthStencilDesc.DepthEnable = depthEnable;
		if (depthEnable) {
			depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
			depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
			graphicsPipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		}
		depthStencilDesc.StencilEnable = FALSE;

		graphicsPipelineDesc.DepthStencilState = depthStencilDesc;


		//
		// �T���v���f�B�X�N�̐ݒ�
		//
		graphicsPipelineDesc.SampleDesc.Count = 1;
		graphicsPipelineDesc.SampleDesc.Quality = 0;


		//
		// ���̑�
		//

		graphicsPipelineDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;// �J�b�g�l�̓i�V

		// ����Ȃ��Ƃ܂���
		graphicsPipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;



		ID3D12PipelineState* tmp = nullptr;
		if (FAILED(device->CreateGraphicsPipelineState(&graphicsPipelineDesc, IID_PPV_ARGS(&tmp))))
		{
			THROW_PDX12_EXCEPTION("failed CreateGraphicsPipelineState");
		}

		return release_unique_ptr<ID3D12PipelineState>{tmp};
	}

	inline release_unique_ptr<ID3D12PipelineState> create_compute_pipeline(ID3D12Device* device,
		ID3D12RootSignature* rootSignature, ID3DBlob* computeShader)
	{
		D3D12_COMPUTE_PIPELINE_STATE_DESC computePipelineDesc{};
		computePipelineDesc.CS.pShaderBytecode = computeShader->GetBufferPointer();
		computePipelineDesc.CS.BytecodeLength = computeShader->GetBufferSize();
		computePipelineDesc.pRootSignature = rootSignature;

		ID3D12PipelineState* tmp = nullptr;
		if (FAILED(device->CreateComputePipelineState(&computePipelineDesc, IID_PPV_ARGS(&tmp))))
		{
			THROW_PDX12_EXCEPTION("failed CreateComputePipelineState");
		}

		return release_unique_ptr<ID3D12PipelineState>{tmp};
	}
}