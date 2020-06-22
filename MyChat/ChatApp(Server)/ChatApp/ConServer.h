#pragma once
#include "TCPConnection.h"
#include <iostream>
#include <Windows.h>
#include <map>

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
	void PrintHelp();
	virtual void CloseSocket();
	virtual void Receive(std::string& message,std::string& name , int index);
	virtual bool Send(const std::string& message,const std::string sender,const int index,int from);
	void AddUsername(std::string& username, int& socket);
	std::string GetClientName(int socket);
	int GetClientSize();
	void RemoveClientName(int socket);
	//THREADS
	 void sendThread();
	 void recvThread();
	 void lisnThread();
	 //******
	 void ExitSession();
	 virtual bool OpenSocket();
	 void SetColor(const Color col);
	 void SetColor(const int col);
     void CloseSocket(const int socketNumber);
	 void ForwardMessage(const std::string message,const std::string name,const int fromClient);
private:
	HANDLE consoleHandle;
	SDLNet_SocketSet m_socketSet;
	TCPsocket m_listenSocket;
	TCPsocket m_socket[MAX_CLIENTS];
	std::string m_welcomeMessage = "Welcome to My Chat.write exit to end the session";

	//I want to Create a map so I can Store The Client Names there
	std::map<int , std::string> m_ClientNames;

	int  m_color = 0; //Set the server Color to 0
};

