#include <iostream>
#include "windows.h"
#include "WebSocket.h"
#include "Window.h"
#include "discord.h"
#include "ixwebsocket/IXWebSocket.h"
#include "nlohmann/json.hpp"
#include <winhttp.h>

#include "Quests.h"

#pragma comment(lib, "winhttp.lib")

std::string GetWebSocketUrl() {
	// Retry for up to 30 seconds
	for (int i = 0; i < 30; i++) {
		HINTERNET hSession = WinHttpOpen(L"CDP/1.0",
			WINHTTP_ACCESS_TYPE_NO_PROXY, nullptr, nullptr, 0);
		HINTERNET hConnect = WinHttpConnect(hSession,
			L"localhost", 9222, 0);
		HINTERNET hRequest = WinHttpOpenRequest(hConnect,
			L"GET", L"/json", nullptr, nullptr, nullptr, 0);

		if (WinHttpSendRequest(hRequest, nullptr, 0, nullptr, 0, 0, 0) &&
			WinHttpReceiveResponse(hRequest, nullptr)) {
				std::string body;
				DWORD bytesRead = 0;
				char buf[4096];
				while (WinHttpReadData(hRequest, buf, sizeof(buf), &bytesRead) && bytesRead)
					body.append(buf, bytesRead);

				WinHttpCloseHandle(hRequest);
				WinHttpCloseHandle(hConnect);
				WinHttpCloseHandle(hSession);

				if (!body.empty()) {
					auto arr = nlohmann::json::parse(body);
					// Find the main Discord page, not devtools
					for (auto& target : arr) {
						std::string url = target.value("url", "");
						if (url.find("discord.com") != std::string::npos) {
							return target["webSocketDebuggerUrl"];
						}
					}
				}
			}

		WinHttpCloseHandle(hRequest);
		WinHttpCloseHandle(hConnect);
		WinHttpCloseHandle(hSession);

		std::cout << "Waiting for Discord... (" << i + 1 << "/30)\n";
		Sleep(1000);
	}
	return "";
}


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow) {
	Window wnd{hInst};
	MSG msg{};

	LaunchDiscord();

	// setup websocket
	std::string websocketUrl = GetWebSocketUrl();
	std::cout << websocketUrl << std::endl;
	WebSocket webSocket{websocketUrl};
	Quests quests{webSocket};

	webSocket.connect();

	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return static_cast<int>(msg.wParam);
}