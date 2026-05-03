//
// Created by Rin on 5/3/2026.
//

#ifndef DISCORDQUESTSPOOFER_QUESTMANAGER_H
#define DISCORDQUESTSPOOFER_QUESTMANAGER_H
#include "WebSocket.h"
#include "nlohmann/json.hpp"

typedef nlohmann::basic_json<> quest;
typedef std::vector<quest> questList;

class Quests {
public:
	Quests(WebSocket& ws);

	void updateQuests() const;
	quest getQuest();
	void doQuest();

private:
	WebSocket& m_ws;
	std::vector<quest> m_quests;
};


#endif // DISCORDQUESTSPOOFER_QUESTMANAGER_H
