//
// Created by Rin on 5/3/2026.
//

#ifndef DISCORDQUESTSPOOFER_WEBSOCKET_H
#define DISCORDQUESTSPOOFER_WEBSOCKET_H
#include <functional>
#include <string>
#include <ixwebsocket/IXWebSocket.h>

typedef std::function<void(const std::string& message)> callback_t;
typedef std::function<void()> onOpen_t;

class WebSocket {
public:
	explicit WebSocket(const std::string& url);
	~WebSocket();

	void connect();
	void disconnect();

	void send(const std::string& message);
	void setOnMessage(const callback_t & callback);
	void setOnOpen(const onOpen_t & callback);
	void waitUntilDone() const;

private:
	std::string m_url;
	onOpen_t m_onOpen;
	callback_t m_onMessage;
	ix::WebSocket m_webSocket;
};


#endif // DISCORDQUESTSPOOFER_WEBSOCKET_H
