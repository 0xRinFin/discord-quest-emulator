//
// Created by Rin on 5/2/2026.
//

#include "Window.h"
#include <cstdio>

LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg) {
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}
	return DefWindowProc(hwnd, msg, wp, lp);
}

Window::Window(HINSTANCE hInst) {
	WNDCLASSEX wc = {};
	wc.cbSize        = sizeof(WNDCLASSEX);
	wc.lpfnWndProc   = Window::WndProc;   // your message handler
	wc.hInstance     = hInst;
	wc.lpszClassName = "HelloWorldClass";
	wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);

	if (!RegisterClassEx(&wc)) {
		printf("RegisterClassEx failed: %lu\n", GetLastError());
		return;
	}

	m_hwnd = CreateWindowEx(
		0,
		"HelloWorldClass",
		"Where Winds Meet",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		400, 400,
		nullptr, nullptr, hInst, this
	);
	if (!m_hwnd) {
		printf("CreateWindowEx failed: %lu\n", GetLastError());
	}

	printf("Window::Window()");
	ShowWindow(m_hwnd, true);
}
