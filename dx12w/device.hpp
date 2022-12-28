#pragma once
#include<d3d12.h>
#include<dxgi1_4.h>
#include<stdexcept>
#include<iterator>
#include"utility.hpp"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

namespace dx12w
{
	
	inline release_unique_ptr<ID3D12Device> create_device()
	{

#ifdef _DEBUG
		ID3D12Debug* debugLayer = nullptr;
		if (FAILED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugLayer))))
		{
			THROW_PDX12_EXCEPTION("failed D3D12GetDebugInterface");
		}
		else
		{
			debugLayer->EnableDebugLayer();
			debugLayer->Release();
		}
#endif

		dx12w::release_unique_ptr<IDXGIFactory4> factory{};

		// �t�@�N�g���[�̍쐬
		{
			IDXGIFactory4* tmp = nullptr;
			if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&tmp))))
			{
				THROW_PDX12_EXCEPTION("failed CreateDXGIFactory1");
			}
			factory.reset(tmp);
		}


		dx12w::release_unique_ptr<IDXGIAdapter3> adaptor{};

		// �A�_�v�^�[�̍쐬
		{
			IDXGIAdapter3* tmp = nullptr;
			UINT adapterIndex = 0;
			DXGI_ADAPTER_DESC1 desc{};

			// adapterIndex�ő��������p�ł���A�_�v�^�[���쐬����
			while (true)
			{
				factory->EnumAdapters1(adapterIndex, (IDXGIAdapter1**)&tmp);
				tmp->GetDesc1(&desc);

				// �K�؂ȃA�_�v�^�����������ꍇ
				if (!(desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE))
					break;

				adapterIndex++;
				// ������Ȃ������ꍇ
				if (adapterIndex == DXGI_ERROR_NOT_FOUND)
				{
					THROW_PDX12_EXCEPTION("not found adapter");
				}
			}

			adaptor.reset(tmp);
		}


		dx12w::release_unique_ptr<ID3D12Device> device{};

		// �f�o�C�X�̍쐬
		{
			ID3D12Device* tmp = nullptr;

			// Direct3D�f�o�C�X���Ώۂ���@�\�����ʂ���萔���������񋓂��Ă���
			D3D_FEATURE_LEVEL levels[] = {
				D3D_FEATURE_LEVEL_12_1,
				D3D_FEATURE_LEVEL_12_0,
				D3D_FEATURE_LEVEL_11_1,
				D3D_FEATURE_LEVEL_11_0
			};

			std::size_t i = 0;
			for (; i < std::size(levels); i++)
			{
				if (SUCCEEDED(D3D12CreateDevice(adaptor.get(), levels[i], IID_PPV_ARGS(&tmp))))
				{
					// �쐬�ł����烋�[�v�𔲂���
					break;
				}
			}

			// �����ł��Ȃ������ꍇ�͗�O�𓊂���
			if (i == std::size(levels))
			{
				THROW_PDX12_EXCEPTION("failed D3D12CreateDevice");
			}

			device.reset(tmp);
		}

		return device;
	}

}