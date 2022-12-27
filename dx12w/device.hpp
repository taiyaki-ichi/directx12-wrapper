#pragma once
#include<d3d12.h>
#include<dxgi1_4.h>
#include<stdexcept>
#include<iterator>
#include"utility.hpp"

namespace dx12w
{
	
	dx12w::release_unique_ptr<ID3D12Device> create_device();

}