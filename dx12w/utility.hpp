#pragma once
#include<stdexcept>
#include<sstream>
#include<memory>
#include<utility>
#include<algorithm>
#include<array>
#include<d3d12.h>
#include<dxgi1_4.h>

#include<iostream>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

namespace dx12w
{

	inline void throw_exception(char const* fileName, int line, char const* func, char const* str)
	{
		std::stringstream ss{};
		ss << fileName << " , " << line << " , " << func << " : " << str << "\n";
		throw std::runtime_error{ ss.str() };
	}

#define THROW_PDX12_EXCEPTION(s)	throw_exception(__FILE__,__LINE__,__func__,s);


	// デストラクタでReleaseを呼び指すスマートポインタを定義する際に使用する
	template<typename T>
	struct release_deleter {
		void operator()(T* ptr) {
			ptr->Release();
		}
	};

	// ComPtrってマイクロソフトのスマートポインタを使うのが無難っぽいけど
	// ドキュメントとか読むのめんどくさいので
	template<typename T>
	using release_unique_ptr = std::unique_ptr<T, release_deleter<T>>;



	// 戻り値はsrc、dstの順
	inline std::pair<D3D12_TEXTURE_COPY_LOCATION, D3D12_TEXTURE_COPY_LOCATION> get_texture_copy_location(
		ID3D12Device* device, ID3D12Resource* srcResource, ID3D12Resource* dstResource)
	{
		D3D12_TEXTURE_COPY_LOCATION srcLocation{};
		D3D12_TEXTURE_COPY_LOCATION dstLocation{};

		auto const dstDesc = dstResource->GetDesc();
		auto const width = dstDesc.Width;
		auto const height = dstDesc.Height;
		auto const uploadResourceRowPitch = srcResource->GetDesc().Width / height;

		srcLocation.pResource = srcResource;
		srcLocation.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
		{
			D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint{};
			UINT nrow;
			UINT64 rowsize, size;
			device->GetCopyableFootprints(&dstDesc, 0, 1, 0, &footprint, &nrow, &rowsize, &size);
			srcLocation.PlacedFootprint = footprint;
		}
		srcLocation.PlacedFootprint.Offset = 0;
		srcLocation.PlacedFootprint.Footprint.Width = static_cast<UINT>(width);
		srcLocation.PlacedFootprint.Footprint.Height = height;
		srcLocation.PlacedFootprint.Footprint.Depth = 1;
		srcLocation.PlacedFootprint.Footprint.RowPitch = static_cast<UINT>(uploadResourceRowPitch);
		srcLocation.PlacedFootprint.Footprint.Format = dstDesc.Format;

		dstLocation.pResource = dstResource;
		dstLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
		dstLocation.SubresourceIndex = 0;

		return { std::move(srcLocation),std::move(dstLocation) };
	}


	template<typename T>
	inline constexpr T alignment(T size, T alignment) {
		if (size % alignment == 0)
			return size;
		else
			return size + alignment - size % alignment;
	}

}