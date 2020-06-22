#pragma once
#include "TCPConnection.h"
#include <iostream>
#include <Windows.h>


enum class Color { DEFAULT = 15, RED = 12, GREEN = 10, BLUE = 9 };
const int MAX_CLIENTS = 10;


class ConServer :public TCPConnection
{
public :
	ConServer();
public:
	const int GetMaxClients();

	void ListenSocket();
	int GetFreeSocket();
	TCPsocket GetSocket(const int index);

	virtual void CloseSocket();
	virtual void Receive(std::string& message,std::string& name,const int index);
	virtual bool Send(const std::string& message,const std::string sender,const int index);

	//THREADS
	 void sendThread();
	 void recvThread();
	 void lisnThread();
	 //******
	 void ExitSession();
	 virtual bool OpenSocket();
	 void SetColor(const Color col);
     void CloseSocket(const int socketNumber);
	 void ForwardMessage(const std::string message,const std::string name,const int fromClient);
private:
	HANDLE consoleHandle;
	SDLNet_SocketSet m_set;
	TCPsocket m_listenSocket;
	TCPsocket m_socket[MAX_CLIENTS];
	int  m_color = 0; //Set the server Color to 0
};

