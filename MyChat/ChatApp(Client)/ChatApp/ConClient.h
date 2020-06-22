#pragma once
#include "TCPConnection.h"
#include <iostream>
#include <Windows.h>

enum class Color { DEFAULT = 15, RED = 12, GREEN = 10, BLUE = 9 };

class ConClient :public TCPConnection
{
public:
	ConClient();
	~ConClient();
public:

	virtual void CloseSocket();
	virtual void Receive(std::string& message, std::string& username, int& color);
	virtual bool Send(const std::string& message);

	//THREADS
	void sendThread();
	void recvThread();
	//******

	void ExitSession();
	virtual bool OpenSocket();
	void SetColor(const int col);
	void SetColor(const Color col);

private:
	HANDLE consoleHandle;
	TCPsocket m_socket;
	IPaddress m_ip;
};

