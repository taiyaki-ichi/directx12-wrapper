#pragma once
#include<stdexcept>
#include<sstream>
#include<memory>
#include<utility>
#include<algorithm>
#include<array>
#include<cassert>
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


	template<typename T>
	inline constexpr T alignment(T size, T alignment) {
		if (size % alignment == 0)
			return size;
		else
			return size + alignment - size % alignment;
	}

}