#pragma once
#include"dx12w_utility.hpp"
#include<d3d12.h>
#include<dxgi1_4.h>
#include<d3dcompiler.h>
#include<string>
#include<Windows.h>
#include<vector>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")

namespace dx12w
{
	// ファイル名、関数名、インクルードオプション
	// 関数内でシェーダファイルをコンパイルしているので軽くはないと思う
	inline release_unique_ptr<ID3DBlob> compile_shader(const wchar_t* fileName, const char* funcName, const char* includeOption)
	{

		ID3DBlob* tmp = nullptr;
		ID3DBlob* errorBlob = nullptr;
		auto resultValue = D3DCompileFromFile(
			fileName,
			nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
			funcName, includeOption,
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
			0, &tmp, &errorBlob);

		if (FAILED(resultValue))
		{
			if (resultValue == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND)) {
				THROW_PDX12_EXCEPTION("file not found");
			}
			else {
				std::string errstr;
				errstr.resize(errorBlob->GetBufferSize());
				std::copy_n((char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), errstr.begin());
				errorBlob->Release();

				THROW_PDX12_EXCEPTION(errstr.data());
			}
		}

		return release_unique_ptr<ID3DBlob>{tmp};
	}

	// バイナリの読み込み
	inline std::vector<std::uint8_t> load_blob(std::istream& in)
	{
		// ストリームの最後に移動
		in.seekg(0, std::ios_base::end);

		// 大きさの取得
		auto const size = in.tellg();

		// 初期化
		std::vector<std::uint8_t> result(size);

		// 先頭に移動
		in.seekg(0, std::ios_base::beg);

		// 読み込み
		in.read(reinterpret_cast<char*>(result.data()), size);

		return result;
	}
}