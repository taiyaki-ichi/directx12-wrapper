#pragma once
#include<d3d12.h>
#include<dxgi1_4.h>

namespace pdx12
{
	//
	// d3d12.h�̃r���[���쐬����֐��ł�_VIEW_DESC�̍\���̂�ݒ肵�Ȃ���΂����Ȃ�
	// ���������߂�ǂ������̂ł��ꂼ��ɑΉ������֐����`����
	// ���Ԃ�S���ԗ��ł��Ă��Ȃ�
	//


	// sizeInBytes��256�ɃA���C�����g���ĂȂ��ƃG���[�o��̂Œ���
	void create_CBV(ID3D12Device* device, D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle, ID3D12Resource* resource, UINT sizeInBytes);


	void create_texture2D_SRV(ID3D12Device* device, D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle, ID3D12Resource* resource, DXGI_FORMAT format,
		UINT mipLevels, UINT mostDetailedMip, UINT planeSline, float resourceMinLODClamp);

	void create_texture2D_array_SRV(ID3D12Device* device, D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle, ID3D12Resource* resource, DXGI_FORMAT format,
		UINT arraySize, UINT firstArraySlice, UINT mipLevels, UINT mostDetailedMip, UINT planeSlice, float resourceMinLODClamp);


	void create_texture_cube_SRV(ID3D12Device* device, D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle, ID3D12Resource* resource, DXGI_FORMAT format,
		UINT arraySize, UINT firstArraySlice, UINT mipLevels, UINT mostDetailedMip, UINT planeSlice, float resourceMinLODClamp);

	void create_buffer_SRV(ID3D12Device* device, D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle, ID3D12Resource* resource,
		UINT numElements, UINT structureByteStride, UINT firstElement, D3D12_BUFFER_SRV_FLAGS flags);

	void create_texture2D_UAV(ID3D12Device* device, D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle, ID3D12Resource* resource, DXGI_FORMAT format,
		ID3D12Resource* counterResource, UINT mipSlice, UINT planeSlice);

	void create_texture1D_UAV(ID3D12Device* device, D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle, ID3D12Resource* resource, DXGI_FORMAT format,
		ID3D12Resource* counterResource, UINT mipSlice);

	void create_buffer_UAV(ID3D12Device* device, D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle, ID3D12Resource* resource,
		ID3D12Resource* counterResource, UINT numElements, UINT structureByteStride, UINT firstElement, UINT counterOffsetInBytes, D3D12_BUFFER_UAV_FLAGS flags);

	void create_texture2D_array_UAV(ID3D12Device* device, D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle, ID3D12Resource* resource, DXGI_FORMAT format,
		ID3D12Resource* counterResource, UINT arraySize, UINT firstArraySlice, UINT mipSlice, UINT planeSlice);

	void create_texture2D_DSV(ID3D12Device* device, D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle, ID3D12Resource* resource, DXGI_FORMAT format,
		UINT mipSlice);


	void create_texture2D_array_DSV(ID3D12Device* device, D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle, ID3D12Resource* resource, DXGI_FORMAT format,
		UINT arraySize, UINT firstArraySlice, UINT mipSlice);

	void create_texture2D_RTV(ID3D12Device* device, D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle, ID3D12Resource* resource, DXGI_FORMAT format,
		UINT mipSlice, UINT planeSlice);

	void create_texture2D_array_RTV(ID3D12Device* device, D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle, ID3D12Resource* resource, DXGI_FORMAT format,
		UINT arraySize, UINT firstArraySlice, UINT mipSlice, UINT planeSlice);
}