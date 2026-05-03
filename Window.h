//
// Created by Rin on 5/2/2026.
//

#ifndef DISCORDQUESTSPOOFER_WINDOW_H
#define DISCORDQUESTSPOOFER_WINDOW_H
#include <windows.h>


class Window {
public:
	HWND m_hwnd;
	Window(HINSTANCE hInst);

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
};


#endif // DISCORDQUESTSPOOFER_WINDOW_H
