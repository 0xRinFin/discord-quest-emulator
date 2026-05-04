//
// Created by Rin on 5/3/2026.
//

#include "Quests.h"
#include <iostream>
#include <string>

#include "Window.h"

std::tuple<std::string, std::string> getExeFile(const std::string& exePath) {
	std::vector<std::string> strings;
	std::istringstream f(exePath);
	std::string s;
	while (getline(f, s, '/')) {
		strings.push_back(s);
	}

	std::string path = "executables/";
	for (int i = 0; i < strings.size(); i++) {
		if (i == strings.size() - 1)
			break;

		path += strings[i];
	}

	std::filesystem::create_directories(path);
	std::string last_element = strings[strings.size() - 1];

	return {last_element, path};
}

Quests::Quests(WebSocket &ws) : m_ws(ws) {
	ws.setOnMessage([&](const std::string& msg) {
		auto json = nlohmann::json::parse(msg);
		if (json.contains("id") && json["id"] == 1) {
			std::string quest_info_str = json["result"]["result"]["value"];
			// std::string quest_info_str = R"RAWR({"aliases":["Delta Force","Delta Force Demo"],"bot":{"accent_color":null,"avatar":"4bdf24b519467ca45422f7337d093ea1","avatar_decoration_data":null,"banner":null,"banner_color":null,"bot":true,"clan":null,"collectibles":null,"discriminator":"3549","display_name_styles":null,"global_name":null,"id":"1314682894106497096","primary_guild":null,"public_flags":0,"username":"Delta Force"},"bot_public":true,"bot_require_code_grant":false,"cover_image":"f3430d803784f5f42365eefbe8d89add","description":"","executables":[{"is_launcher":false,"name":"win64/deltaforceclient-win64-shipping.exe","os":"win32"},{"is_launcher":false,"name":"launcher/df_launcher_global.exe","os":"win32"},{"is_launcher":false,"name":"deltaforceclient.exe","os":"win32"},{"is_launcher":false,"name":"df_garena/deltaforceclient.exe","os":"win32"},{"is_launcher":false,"name":"launcher/df_launcher.exe","os":"win32"}],"flags":0,"game_data_overrides":{"companies":[],"platforms":[],"screenshot_hashes":[],"shop_collection_ids":[],"steam_recent_rating":52,"steam_recent_rating_count":3495,"trailer_asset_ids":[],"websites":[]},"hook":true,"icon":"4bdf24b519467ca45422f7337d093ea1","id":"1314682894106497096","integration_types_config":{"0":{}},"is_discoverable":false,"is_monetized":false,"is_verified":false,"linked_games":[{"application":{"bot_public":true,"bot_require_code_grant":false,"cover_image":"a22c7732ed7d4c0327970b2a05c55408","description":"","flags":566272,"hook":true,"icon":"a22c7732ed7d4c0327970b2a05c55408","id":"1210430430172680212","install_params":{"permissions":"0","scopes":["bot","applications.commands"]},"integration_types_config":{"0":{}},"is_discoverable":false,"is_monetized":false,"is_verified":false,"linked_games":[{"id":"1314682894106497096","type":1}],"name":"Delta Force","storefront_available":false,"summary":"","type":null,"verify_key":"c123c700eeeba62452c1fac483d214b2e81fb6a70b4c7a718dd255766e071f41"},"id":"1210430430172680212","type":1}],"name":"Delta Force","storefront_available":false,"summary":"","third_party_skus":[{"distributor":"opencritic","id":"17914","sku":"17914"},{"distributor":"playstation","id":"PPSA28884_00","sku":"PPSA28884_00"},{"distributor":"playstation","id":"PPSA31391_00","sku":"PPSA31391_00"},{"distributor":"xbox_title","id":"1854680102","sku":"1854680102"},{"distributor":"xbox","id":"9NRMZXGKP4NM","sku":"9NRMZXGKP4NM"},{"distributor":"playstation","id":"PPSA28883_00","sku":"PPSA28883_00"},{"distributor":"steam","id":"2507950","sku":"2507950"},{"distributor":"epic","id":"3E2C28241611405AA231308A39BEE638","sku":"3E2C28241611405AA231308A39BEE638"},{"distributor":"playstation","id":"PPSA28881_00","sku":"PPSA28881_00"},{"distributor":"playstation","id":"PPSA28882_00","sku":"PPSA28882_00"},{"distributor":"gdco","id":"99991879","sku":"99991879"},{"distributor":"gop","id":"114150","sku":"114150"},{"distributor":"igdb","id":"262186","sku":"262186"}],"type":5,"verify_key":"7c076edb6b9e0fc409acca53b59f987b6826b500f5a3f4282f86ed73391fd9ba"})RAWR";

			std::cout << quest_info_str << std::endl;
			auto quest_info = nlohmann::json::parse(quest_info_str);

			m_quests.push_back(quest_info);
			doQuest();
		}
	});

	ws.setOnOpen([&](){Quests::updateQuests();});
}

quest Quests::getQuest() {
	return m_quests[m_quests.size()-1];
}

void Quests::doQuest() {
	if (m_quests.size() == 0) {
		std::cerr << "Quest list empty." << std::endl;
		return;
	}

	const quest quest_info = getQuest();
	auto executable_info = quest_info["executables"][0];
	auto game_name = std::filesystem::path(executable_info["name"]);
	const auto exeInfo = getExeFile(game_name.string());

	const std::string exeName = std::get<0>(exeInfo);
	const std::string exeDirectoryPath = std::get<1>(exeInfo);

	char exePath[MAX_PATH];
	GetModuleFileNameA(nullptr, exePath, MAX_PATH);

	std::filesystem::path workerPath = std::filesystem::path(exePath).parent_path();
	workerPath /= exeDirectoryPath;

	workerPath /= exeName.c_str();
	std::string workerStr = workerPath.string();
	const char* charPath = workerStr.c_str();
	CopyFileA(exePath, charPath, FALSE);
	ShellExecuteA(nullptr, "open", charPath, "--worker", nullptr, SW_SHOW);
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
		const taskConfig = quest.config.taskConfig ?? quest.config.taskConfigV2
		if(taskName === "WATCH_VIDEO" || taskName === "WATCH_VIDEO_ON_MOBILE") {
			quest = quests.pop()
		}

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
