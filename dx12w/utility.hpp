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

namespace dx12w
{

	void throw_exception(char const* fileName, int line, char const* func, char const* str);

#define THROW_PDX12_EXCEPTION(s)	throw_exception(__FILE__,__LINE__,__func__,s);


	// �f�X�g���N�^��Release���Ăюw���X�}�[�g�|�C���^���`����ۂɎg�p����
	template<typename T>
	struct release_deleter {
		void operator()(T* ptr) {
			ptr->Release();
		}
	};

	// ComPtr���ă}�C�N���\�t�g�̃X�}�[�g�|�C���^���g���̂�������ۂ�����
	// �h�L�������g�Ƃ��ǂނ̂߂�ǂ������̂�
	template<typename T>
	using release_unique_ptr = std::unique_ptr<T, release_deleter<T>>;



	// �߂�l��src�Adst�̏�
	std::pair<D3D12_TEXTURE_COPY_LOCATION, D3D12_TEXTURE_COPY_LOCATION> get_texture_copy_location(
		ID3D12Device* device, ID3D12Resource* srcResource, ID3D12Resource* dstResource);


	template<typename T>
	inline constexpr T alignment(T size, T alignment) {
		if (size % alignment == 0)
			return size;
		else
			return size + alignment - size % alignment;
	}

}