#pragma once
#include<Windows.h>

namespace dx12w
{
	// デフォルトのコールバック関数
	inline LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// TODO: 32ビットの環境だとwindow_nameの型がcharでなければ行けないので対応する必要あり
	inline HWND create_window(wchar_t const* window_name, LONG width, LONG height, WNDPROC wnd_proc = WndProc);

	// ウィンドウについてのメッセージを処理し更新する
	// ウィンドウが閉じられた場合はfalseを返す. 通常はtrueを返す
	inline bool update_window();


	//
	// 以下、実装
	//

	// メッセージを処理するためのコールバック関数を設定するために
	// その関数のポインタが必要になるので定義している
	// この関数は外部から参照される必要がないので無名名前空間でくくった方がいいかも?
	inline LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		//　ウィンドウが破壊された時
		if (msg == WM_DESTROY) {
			// この関数がメッセージキューにWM_QUITを投げる
			PostQuitMessage(0);
			return 0;
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	inline HWND create_window(wchar_t const* window_name, LONG width, LONG height, WNDPROC wnd_proc)
	{
		HINSTANCE hInstance = GetModuleHandle(nullptr);

		// ウィンドウクラス設定
		WNDCLASSEX wcex{};
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.lpfnWndProc = wnd_proc;
		wcex.lpszClassName = window_name;
		wcex.hInstance = hInstance;

		// ウィンドウクラスの登録
		RegisterClassEx(&wcex);

		// ウィンドウサイズの設定
		RECT rect{ 0,0,width,height };

		// TODO: CreateWindowが64bit環境ではCreateWindowW, 32bit環境ではCreateWindowAに展開されるので
		// 文字のサイズについて対応が必要
		HWND hwnd = CreateWindow(
			wcex.lpszClassName,			// クラス名
			window_name,				// タイトルバー
			WS_OVERLAPPEDWINDOW,		// タイトルバーと境界線があるウィンドウ
			CW_USEDEFAULT,				// x座標
			CW_USEDEFAULT,				// y座標
			rect.right - rect.left,		// ヨコ
			rect.bottom - rect.top,		// タテ
			nullptr,					// 親ウィンドウ
			nullptr,					// メニューハンドル
			wcex.hInstance,				// 呼び出しアプリケーションハンドル
			nullptr						// 追加パラメータ
		);

		// ウィンドウの表示
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