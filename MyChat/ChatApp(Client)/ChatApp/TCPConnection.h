#pragma once
#include <string>
#include <SDL.h>
#include <SDL_net.h>
#include "Utils.h"


class TCPConnection
{
public:
	TCPConnection();

public:
	virtual bool OpenSocket();
	std::string PrintUsername();
	bool Initialize(Uint16 port);
	void SetUsername(std::string& username);

	virtual bool Send( std::string& message);
	virtual void Receive(std::string& message, std::string& username) {};
	virtual void Receive(std::string& message, std::string& username, int& color) {};

	void ShutDown();
	virtual void CloseSocket();

private :
	IPaddress m_ip;
	TCPsocket m_socket;
	std::string m_username;


};

