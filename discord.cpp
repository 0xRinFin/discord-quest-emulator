//
// Created by Rin on 5/3/2026.
//

#include <string>
#include "Window.h"
#include "discord.h"

#include <iostream>
#include <ostream>

void LaunchDiscord() {
	WIN32_FIND_DATAA fd;
	std::string discordPath = std::string(getenv("LOCALAPPDATA")) + "\\Discord\\";
	std::string searchPath = discordPath + "app-*";
	HANDLE hFind = FindFirstFileA(searchPath.c_str(), &fd);

	std::cout << hFind << std::endl;

	FindNextFileA(hFind, &fd);
	if (hFind == INVALID_HANDLE_VALUE) {
		std::cout << "Not found app folder" << std::endl;
	}
	std::string latestApp = fd.cFileName;
	std::cout << "Found app folder : " << latestApp << std::endl;

	std::string exePath = discordPath + latestApp + "\\Discord.exe";
	std::string cmdLine = exePath + " --remote-debugging-port=9222";

	std::string cmd = "cmd.exe /c start \"\" \"" + exePath + "\" --remote-debugging-port=9222 --remote-allow-origins=*";
	WinExec(cmd.c_str(), SW_HIDE);
}