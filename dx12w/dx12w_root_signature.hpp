#pragma once
#include<d3d12.h>
#include"dx12w_utility.hpp"
#include<vector>
#include<array>
#include<numeric>

#pragma comment(lib,"d3d12.lib")

namespace dx12w
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
	inline release_unique_ptr<ID3D12RootSignature> create_root_signature(ID3D12Device* device,
		std::vector<std::vector<descriptor_range_type>> const& descriptorRangeTypes, std::vector<static_sampler> const& staticSamplers)
	{
		std::vector<std::vector<D3D12_DESCRIPTOR_RANGE>> descriptorRanges{};

		// �z��̍\�������ƂɃ��W�X�^�̔ԍ���U��, D3D12_DESCRIPTOR_RANGE������Ă���
		{
			descriptorRanges.reserve(descriptorRangeTypes.size());

			// ���łɊ���U�������W�X�^���L�^����p
			// �f�B�X�N���v�^�����W��ނ�4�Ȃ̂Ŕz��̃T�C�Y��4
			std::array<std::uint32_t, 4> registerNums{};

			for (auto& rangeTypes : descriptorRangeTypes) {
				std::vector<D3D12_DESCRIPTOR_RANGE> ranges{};
				ranges.reserve(rangeTypes.size());

				for (auto& t : rangeTypes) {
					D3D12_DESCRIPTOR_RANGE descriptorRange{};
					descriptorRange.NumDescriptors = t.num;
					descriptorRange.RangeType = static_cast<D3D12_DESCRIPTOR_RANGE_TYPE>(t.descriptorRangeType);
					descriptorRange.BaseShaderRegister = registerNums[static_cast<D3D12_DESCRIPTOR_RANGE_TYPE>(t.descriptorRangeType)];
					descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

					ranges.push_back(std::move(descriptorRange));

					// �g�p�ς݃��W�X�^�̃J�E���g���X�V����
					registerNums[static_cast<D3D12_DESCRIPTOR_RANGE_TYPE>(t.descriptorRangeType)] += t.num;
				}

				descriptorRanges.push_back(std::move(ranges));
			}
		}


		std::vector<D3D12_ROOT_PARAMETER> descriptorTables{};

		// descriptorRanges�����ƂɃf�B�X�N���v�^�e�[�u�����쐬
		{
			descriptorTables.reserve(descriptorRanges.size());

			for (auto& r : descriptorRanges) {
				D3D12_ROOT_PARAMETER tmp{};
				tmp.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
				tmp.DescriptorTable.pDescriptorRanges = r.data();
				tmp.DescriptorTable.NumDescriptorRanges = static_cast<UINT>(r.size());
				tmp.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

				descriptorTables.push_back(std::move(tmp));
			}
		}


		std::vector<D3D12_STATIC_SAMPLER_DESC> staticSamplerDescs(staticSamplers.size());

		{
			for (std::uint32_t i = 0; i < staticSamplers.size(); i++)
			{
				staticSamplerDescs[i].Filter = staticSamplers[i].filter;
				staticSamplerDescs[i].AddressU = staticSamplers[i].address_u;
				staticSamplerDescs[i].AddressV = staticSamplers[i].address_v;
				staticSamplerDescs[i].AddressW = staticSamplers[i].address_w;
				staticSamplerDescs[i].ComparisonFunc = staticSamplers[i].comparison_func;

				staticSamplerDescs[i].MaxAnisotropy = staticSamplers[i].max_anisotropy;

				staticSamplerDescs[i].MinLOD = staticSamplers[i].min_LOD;
				staticSamplerDescs[i].MaxLOD = staticSamplers[i].max_LOD;

				staticSamplerDescs[i].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
				staticSamplerDescs[i].ShaderRegister = i;
			}
		}



		D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
		rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		rootSignatureDesc.NumParameters = static_cast<UINT>(descriptorTables.size());
		rootSignatureDesc.pParameters = descriptorTables.size() > 0 ? descriptorTables.data() : nullptr;
		rootSignatureDesc.NumStaticSamplers = static_cast<UINT>(staticSamplerDescs.size());
		rootSignatureDesc.pStaticSamplers = staticSamplerDescs.size() > 0 ? staticSamplerDescs.data() : nullptr;


		ID3DBlob* rootSigBlob = nullptr;
		ID3DBlob* errorBlob = nullptr;
		release_unique_ptr<ID3D12RootSignature> result{};

		if (FAILED(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob)))
		{
			// �G���[�̏��̎��o��
			std::string errstr{};
			errstr.resize(errorBlob->GetBufferSize());
			std::copy_n((char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), errstr.begin());
			errorBlob->Release();

			THROW_PDX12_EXCEPTION(errstr.data());
		}

		ID3D12RootSignature* tmp = nullptr;
		if (FAILED(device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&tmp))))
		{
			THROW_PDX12_EXCEPTION("failed CreateRootSignature");
		}

		result.reset(tmp);

		// �����g��Ȃ�������
		rootSigBlob->Release();

		return result;
	}

	// �w���p
	// �f�X�N���v�^�̐��𐔂���
	template<typename T>
	decltype(auto) calc_descriptor_num(const T& t)
	{
		if constexpr (std::is_same_v<std::remove_cv_t<T>, descriptor_range_type>)
			return t.num;
		else {
			UINT sum = 0;
			std::for_each(std::begin(t), std::end(t), [&sum](const auto& v) {sum += calc_descriptor_num(v); });
			return sum;
		}
	}
}