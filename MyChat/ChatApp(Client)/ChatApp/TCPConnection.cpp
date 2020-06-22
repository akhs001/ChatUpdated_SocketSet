#include "TCPConnection.h"
#include <iostream>

TCPConnection::TCPConnection()
{
	m_ip = { 0, 0 };
	m_socket = nullptr;
	m_username = "";
}


bool TCPConnection::Initialize(Uint16 port)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		Utils::Log( "Error initializing SDL" );
		system("pause");
		return false;
	}

	if (SDLNet_Init() == -1)
	{
		Utils::Log("Error initializing SDL net" );
		system("pause");
		return false;
	}
	return true;
}

bool TCPConnection::OpenSocket()
{
	//Get the IP for connection
	char address[30];
	Utils::Log("Give the Server Ip Address." );
	std::cin >> address;

	//Setup the server and listening to the port 1234
	if (SDLNet_ResolveHost(&m_ip, address, 1234) == -1)
	{
		Utils::Log("Error Connecting to server" );
		system("pause");
		return 0;
	}

	m_socket = SDLNet_TCP_Open(&m_ip);

	if (!m_socket)
	{
		Utils::Log("Could not open socket for comminication");
		system("pause");
		return 0;
	}

	Utils::Log("Connected to Server");
}

std::string TCPConnection::PrintUsername()
{
	return m_username;
}

void TCPConnection::SetUsername(std::string& username)
{
	m_username = username;
}

bool TCPConnection::Send( std::string& message)
{
	std::string newMessage = m_username + ":" + message;

	int length = newMessage.length() + 1;
	//Senbd the message to the client
	
	if ( SDLNet_TCP_Send(m_socket, newMessage.c_str(), length) > length)
	{
		return true;
	}
	else
	return false;
}


void TCPConnection::CloseSocket()
{
	SDLNet_TCP_Close(m_socket);
}

void TCPConnection::ShutDown()
{
	//shutDown Sdl net
	SDLNet_Quit();

	//shutDown Sdl
	SDL_Quit();
}
