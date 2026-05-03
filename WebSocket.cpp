//
// Created by Rin on 5/3/2026.
//

#include "WebSocket.h"

#include <iostream>
#include <chrono>

WebSocket::WebSocket(const std::string &url) : m_url(url) {}
WebSocket::~WebSocket() {
	disconnect();
}

void WebSocket::setOnMessage(const callback_t & callback) { m_onMessage = callback; }
void WebSocket::setOnOpen(const onOpen_t & callback) { m_onOpen = callback; }

void WebSocket::connect() {
	std::cout << "connecting" << std::endl;
	m_webSocket.setUrl(m_url);
	m_webSocket.setExtraHeaders({
		{"Host", "localhost"},
		{"Origin", "http://localhost"}
	});

	m_webSocket.setOnMessageCallback([this](const std::unique_ptr<ix::WebSocketMessage> & web_socket_message) {
		std::cout << static_cast<int>(web_socket_message->type) << std::endl;
		switch (web_socket_message->type) {
			case ix::WebSocketMessageType::Open:
				if (m_onOpen)
					m_onOpen();

				std::cout << "Websocket opened" << std::endl;
				break;
			case ix::WebSocketMessageType::Close:
				std::cout << "Websocket closed" << std::endl;

				break;
			case ix::WebSocketMessageType::Error:
				std::cerr << "WebSocket error: " << web_socket_message->errorInfo.reason << "\n";
				std::cerr << "HTTP status: " << web_socket_message->errorInfo.http_status << "\n";
				std::cout << "Websocket error pls help" << std::endl;

				break;
			case ix::WebSocketMessageType::Message:
				if (m_onMessage)
					m_onMessage(web_socket_message->str);

				std::cout << "Websocket message" << std::endl;
				break;
			default:
				break;
		}
	});

	std::cout << "connected" << std::endl;
	m_webSocket.start();
}

void WebSocket::disconnect() {
	m_webSocket.stop();
}

void WebSocket::send(const std::string &message) {
	m_webSocket.send(message);
}

void WebSocket::waitUntilDone() const {
	while (m_webSocket.getReadyState() != ix::ReadyState::Closed)
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
}
