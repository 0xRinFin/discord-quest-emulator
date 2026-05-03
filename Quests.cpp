//
// Created by Rin on 5/3/2026.
//

#include "Quests.h"
#include <iostream>

#include "Window.h"

Quests::Quests(WebSocket &ws) : m_ws(ws) {
	ws.setOnMessage([&](const std::string& msg) {
		auto json = nlohmann::json::parse(msg);
		if (json.contains("id") && json["id"] == 1) {
			std::string quest_info_str = json["result"]["result"]["value"];
			auto quest_info = nlohmann::json::parse(quest_info_str);

			m_quests.push_back(quest_info);
			doQuest();
		}
	});

	ws.setOnOpen([&](){Quests::updateQuests();});
}

quest Quests::getQuest() {
	std::cout << "ABOUT TO SEGLFAULT" << std::endl;
	std::cout << m_quests[m_quests.size()-1] << std::endl;
	return m_quests[m_quests.size()-1];
}

void Quests::doQuest() {
	if (m_quests.size() == 0) {
		std::cerr << "Quest list empty." << std::endl;
		return;
	}

	const quest quest_info = getQuest();
	auto executable_info = quest_info["executables"][0];
	std::string game_name = static_cast<std::string>(executable_info["name"]);

	char exePath[MAX_PATH];
	GetModuleFileNameA(nullptr, exePath, MAX_PATH);

	char workerPath[MAX_PATH];
	strcpy(workerPath, exePath);

	char* lastSlash = strrchr(workerPath, '\\');
	if (lastSlash) strcpy(lastSlash + 1, game_name.c_str());

	CopyFileA(exePath, workerPath, FALSE);
	ShellExecuteA(nullptr, "open", workerPath, "--worker", nullptr, SW_SHOW);
}

// thanks amia <3 (stole your code)
void Quests::updateQuests() const {
	std::string js = R"(
    (async () => {
        let wpRequire = webpackChunkdiscord_app.push([[Symbol()], {}, r => r]);
        webpackChunkdiscord_app.pop();

        let api = Object.values(wpRequire.c)
            .find(x => x?.exports?.Bo?.get).exports.Bo;
        let QuestsStore = Object.values(wpRequire.c)
            .find(x => x?.exports?.A?.__proto__?.getQuest).exports.A;

        let quests = [...QuestsStore.quests.values()]
            .filter(x => x.userStatus?.enrolledAt &&
                         !x.userStatus?.completedAt &&
                         new Date(x.config.expiresAt).getTime() > Date.now());

        const quest = quests.pop();
        const res = await api.get({
            url: `/applications/public?application_ids=${quest.config.application.id}`
        });

        return JSON.stringify(res.body[0]);
    })()
)";

	nlohmann::json cmd = {
		{"id", 1},
		{"method", "Runtime.evaluate"},
		{"params", {
	        {"expression", js},
			{"returnByValue", true},
			{"awaitPromise", true}
		}}
	};

	m_ws.send(cmd.dump());
}
