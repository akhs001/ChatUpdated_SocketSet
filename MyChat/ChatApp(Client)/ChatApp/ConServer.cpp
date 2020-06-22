#include "ConServer.h"
#include <SDL_net.h>



Color socketColor[4] = { Color::DEFAULT , Color::RED , Color::GREEN , Color::BLUE };

ConServer::ConServer()
{
	consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	//Initialize the sockets
	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		m_socket[i] = nullptr;

	}

	m_listenSocket = nullptr;
}

 void ConServer::ListenSocket() 
{
	//Get a free socket
	int socketToUse = GetFreeSocket();

	while (!m_socket[socketToUse])
	{
		m_socket[socketToUse] = SDLNet_TCP_Accept(m_listenSocket);
		SDL_Delay(1000);
	}
}


 int ConServer::GetFreeSocket()
{
	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		if (m_socket[i] == nullptr)
		{
			return i;
		}
	}
	return -1;
}


TCPsocket ConServer::GetSocket(const int index)
{
	return m_socket[index];
}

const int ConServer::GetMaxClients()
{
	return MAX_CLIENTS;
}

bool ConServer::Send(const std::string& message,const std::string sender,const  int index)
{
	std::string  newMessage;
	//Add name to message
	if (sender == "")
	{
		newMessage = PrintUsername() + ":" + message;
	}

	else
		newMessage = sender + ":" + message;

	int length = newMessage.length() + 1;
	//Senbd the message to the client

	if (m_socket[index] && SDLNet_TCP_Send(m_socket[index], newMessage.c_str(), length) > length)
	{
		return true;
	}

	return false;

}

void ConServer::Receive(std::string& message, std::string& name,const int index)
{
	char response[2000];
	//receiving message

	if ( SDLNet_TCP_Recv(m_socket[index], response, 2000) > 0)
	{
	//USE SPLIT HERE
		std::cout << response << std::endl;
	}
}



void ConServer::CloseSocket()
{
	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		SDLNet_TCP_Close(m_socket[i]);
	}
	SDLNet_TCP_Close(m_listenSocket);
}

void ConServer::sendThread()
{
	//Create the sending thread
		std::string Message = "";
		while (Message != "exit")
		{
			//Send Message
			std::getline(std::cin, Message);
			//Put the Color inside the message
			Message = Message + ":" + std::to_string(m_color);
			//Set the length of the message
			int length = Message.length() + 1;

			for (int i = 0; i < GetMaxClients(); i++)
			{
			Send(Message, "", i);
			}
		}
		//if you write exit
		ExitSession();
	
}

void ConServer::recvThread()
{
	//Create the Receive thread
		std::string Message = "";
		std::string Username = "";
		int currentSocket = 0;

		while (Message != "exit")
		{
			for (int i = 0; i < GetMaxClients(); i++)
			{
				if (m_socket[i] == nullptr) { continue; }
				Message = "";
				Username = "";
				currentSocket = i;
				Receive(Message, Username, i);

				if (Message != "" && Message != "exit")
				{
					//If we receive a message from a Client
					//First we need to Display to server
					std::cout << "               " << Username << ":" << Message << std::endl;	//Display the message

					//And then Forward the message  to everyone else Client
					ForwardMessage(Message, Username, i);
				}
				else if (Message == "exit")
				{
					CloseSocket(i);
					std::cout << "Client disconect.I have to close the socket Number " << currentSocket << std::endl;
				}
				else
				{
					continue;
				}
			}
		}
		//if I Receive  exit i have to End the connection with and close the socket

	}

void ConServer::lisnThread()
{
		//If We dont have more free Sockets stop Listening
		while (GetFreeSocket() != -1)
		{
		ListenSocket();
		}
	
}

void ConServer::ForwardMessage(const std::string message,const std::string name, const int fromClient)
{
	//Forward a client message to All other ClientsI create 

		for (int i = 0; i < GetMaxClients(); i++)
		{
			if (i != fromClient)
			Send(message, name, i);
		}
}

void ConServer::SetColor(const Color col)
{
	//Set the Color of the text

		SetConsoleTextAttribute(consoleHandle, static_cast<WORD> (col));
	
}

void ConServer::CloseSocket(const int socketNumber)
{
	SDLNet_TCP_Close(m_socket[socketNumber]);
}



bool ConServer::OpenSocket()
{

	m_listenSocket = SDLNet_TCP_Open(&GetIp() );

	if (!m_listenSocket)
	{
		std::cout << "Could not open Listen socket for connection" << std::endl;
		system("pause");
		return false;
	}

	return true;
}

void ConServer::ExitSession()
{
	std::string Message = PrintUsername() + " leave the session";
	int length = Message.length() + 1;

	//Send The message to everyone
	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		Send(Message , "" , i);
	}
	

}
