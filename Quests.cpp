//
// Created by Rin on 5/3/2026.
//

#include "Quests.h"
#include <iostream>

Quests::Quests(WebSocket &ws) : m_ws(ws) {
	ws.setOnMessage([&](const std::string& msg) {
		auto json = nlohmann::json::parse(msg);
		if (json.contains("id") && json["id"] == 1) {
			std::string quest_info_str = json["result"]["result"]["value"];
			auto quest_info = nlohmann::json::parse(quest_info_str);

			std::cout << "Result: " << quest_info["id"] << "\n";
		}
	});

	ws.setOnOpen([&](){Quests::updateQuests();});
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
