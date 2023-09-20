#pragma once
#include<Windows.h>

namespace dx12w
{
	// �f�t�H���g�̃R�[���o�b�N�֐�
	inline LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// TODO: 32�r�b�g�̊�����window_name�̌^��char�łȂ���΍s���Ȃ��̂őΉ�����K�v����
	inline HWND create_window(wchar_t const* window_name, LONG width, LONG height, WNDPROC wnd_proc = WndProc);

	// �E�B���h�E�ɂ��Ẵ��b�Z�[�W���������X�V����
	// �E�B���h�E������ꂽ�ꍇ��false��Ԃ�. �ʏ��true��Ԃ�
	inline bool update_window();


	//
	// �ȉ��A����
	//

	// ���b�Z�[�W���������邽�߂̃R�[���o�b�N�֐���ݒ肷�邽�߂�
	// ���̊֐��̃|�C���^���K�v�ɂȂ�̂Œ�`���Ă���
	// ���̊֐��͊O������Q�Ƃ����K�v���Ȃ��̂Ŗ������O��Ԃł�������������������?
	inline LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		//�@�E�B���h�E���j�󂳂ꂽ��
		if (msg == WM_DESTROY) {
			// ���̊֐������b�Z�[�W�L���[��WM_QUIT�𓊂���
			PostQuitMessage(0);
			return 0;
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	inline HWND create_window(wchar_t const* window_name, LONG width, LONG height, WNDPROC wnd_proc)
	{
		HINSTANCE hInstance = GetModuleHandle(nullptr);

		// �E�B���h�E�N���X�ݒ�
		WNDCLASSEX wcex{};
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.lpfnWndProc = wnd_proc;
		wcex.lpszClassName = window_name;
		wcex.hInstance = hInstance;

		// �E�B���h�E�N���X�̓o�^
		RegisterClassEx(&wcex);

		// �E�B���h�E�T�C�Y�̐ݒ�
		RECT rect{ 0,0,width,height };

		// TODO: CreateWindow��64bit���ł�CreateWindowW, 32bit���ł�CreateWindowA�ɓW�J�����̂�
		// �����̃T�C�Y�ɂ��đΉ����K�v
		HWND hwnd = CreateWindow(
			wcex.lpszClassName,			// �N���X��
			window_name,				// �^�C�g���o�[
			WS_OVERLAPPEDWINDOW,		// �^�C�g���o�[�Ƌ��E��������E�B���h�E
			CW_USEDEFAULT,				// x���W
			CW_USEDEFAULT,				// y���W
			rect.right - rect.left,		// ���R
			rect.bottom - rect.top,		// �^�e
			nullptr,					// �e�E�B���h�E
			nullptr,					// ���j���[�n���h��
			wcex.hInstance,				// �Ăяo���A�v���P�[�V�����n���h��
			nullptr						// �ǉ��p�����[�^
		);

		// �E�B���h�E�̕\��
		ShowWindow(hwnd, SW_SHOW);

		return hwnd;
	}


	inline bool update_window()
	{
		MSG msg{};
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
			return false;
		else
			return true;
	}
}