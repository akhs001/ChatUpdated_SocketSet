#include "TCPConnection.h"
#include <iostream>
#include <string>

TCPConnection::TCPConnection()
{
	//Initialize the m_ip variable
	m_IP = { 0, 0 };
}


//Initialize the SDL And SDLNet
bool TCPConnection::Initialize(Uint16 port , IPaddress& ip)
{
	//Initialize the SDL and SDLNet
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		std::cout << "Error initializing SDL" << std::endl;
		system("pause");
		return false;
	}

	if (SDLNet_Init() == -1)
	{
		std::cout << "Error initializing SDL net" << std::endl;
		system("pause");
		return false;
	}

	//Setup the server and listening to the port 1234
	if (SDLNet_ResolveHost(&ip, nullptr , port) == -1)
	{
		std::cout << "Error Creating the server" << std::endl;
		system("pause");
		return 0;
	}

	return true;
}

std::string TCPConnection::PrintUsername()
{
	return m_username;
}

IPaddress& TCPConnection::GetIp()
{
	return m_IP;
}

void TCPConnection::SetUsername(std::string username)
{
	m_username = username;
}

bool TCPConnection::OpenSocket()
{
	return false;
}

bool TCPConnection::Send(std::string& message, std::string sender, int index)
{
	return false;
}

bool TCPConnection::Send(std::string& message)
{
	return false;
}


void TCPConnection::ShutDown()
{
	//shutDown Sdl net
	SDLNet_Quit();

	//shutDown Sdl
	SDL_Quit();
}
