#pragma once
#include<d3d12.h>
#include<dxgi1_4.h>
#include"dx12w_utility.hpp"
#include"dx12w_resource.hpp"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

namespace dx12w
{
	// スワップチェーンの作成
	inline release_unique_ptr<IDXGISwapChain3> create_swap_chain(ID3D12CommandQueue* commandQueue, HWND hwnd, DXGI_FORMAT frameBufferFormat, UINT frameBufferNum)
	{
		release_unique_ptr<IDXGIFactory4> factory{};

		{
			IDXGIFactory4* tmp = nullptr;

#ifdef _DEBUG
			if (FAILED(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&tmp))))
				THROW_PDX12_EXCEPTION("failed CreateDXGIFactory2");
#else
			if (FAILED(CreateDXGIFactory2(0, IID_PPV_ARGS(&tmp))))
				THROW_PDX12_EXCEPTION("failed CreateDXGIFactory2");
#endif

			factory.reset(tmp);
		}


		release_unique_ptr<IDXGISwapChain3> swapChain{};

		{
			IDXGISwapChain3* tmp = nullptr;

			// ウィンドウの大きさを取得する用
			RECT windowRect{};
			GetWindowRect(hwnd, &windowRect);

			DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};
			swapchainDesc.Width = windowRect.right - windowRect.left;
			swapchainDesc.Height = windowRect.bottom - windowRect.top;
			swapchainDesc.Format = frameBufferFormat;
			swapchainDesc.BufferCount = frameBufferNum;
			swapchainDesc.Stereo = false;
			swapchainDesc.SampleDesc.Count = 1;
			swapchainDesc.SampleDesc.Quality = 0;
			swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
			swapchainDesc.Scaling = DXGI_SCALING_STRETCH;
			swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			swapchainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
			swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;


			// ここでCommandQueueが必要なのなんでだろ
			// 同期したりする時には必要そうかな
			if (FAILED(factory->CreateSwapChainForHwnd(commandQueue, hwnd, &swapchainDesc, nullptr, nullptr, (IDXGISwapChain1**)&tmp)))
				THROW_PDX12_EXCEPTION("failed CreateSwapChainForHwnd");

			swapChain.reset(tmp);
		}


		return swapChain;
	}

	// スワップチェーンからフレームバッファの取得
	template<std::size_t FRAME_BUFFER_NUM, typename Container = std::array<dx12w::resource_and_state, FRAME_BUFFER_NUM>>
	inline Container get_frame_buffer_resource(IDXGISwapChain3* swapChain)
	{
		Container result{};
		auto containerIter = std::begin(result);

		for (std::size_t i = 0; i < FRAME_BUFFER_NUM; i++)
		{
			ID3D12Resource* tmp = nullptr;
			swapChain->GetBuffer(static_cast<UINT>(i), IID_PPV_ARGS(&tmp));
			*containerIter = std::make_pair(dx12w::release_unique_ptr<ID3D12Resource>{tmp}, D3D12_RESOURCE_STATE_COMMON);
			containerIter++;
		}

		return result;
	}
}