#pragma once
#include"utility.hpp"
#include<d3d12.h>
#include<dxgi1_4.h>
#include<d3dcompiler.h>
#include<string>
#include<Windows.h>


namespace dx12w
{
	// �t�@�C�����A�֐����A�C���N���[�h�I�v�V����
	// �֐����ŃV�F�[�_�t�@�C�����R���p�C�����Ă���̂Ōy���͂Ȃ��Ǝv��
	release_unique_ptr<ID3DBlob> create_shader(const wchar_t* fileName, const char* funcName, const char* includeOption);
}