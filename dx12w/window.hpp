#pragma once
#include<Windows.h>

namespace dx12w
{
	// TODO: 32�r�b�g�̊�����window_name�̌^��char�łȂ���΍s���Ȃ��̂őΉ�����K�v����
	HWND create_window(wchar_t const* window_name, LONG width, LONG height);

	// �E�B���h�E�ɂ��Ẵ��b�Z�[�W���������X�V����
	// �E�B���h�E������ꂽ�ꍇ��false��Ԃ�. �ʏ��true��Ԃ�
	bool update_window();
}