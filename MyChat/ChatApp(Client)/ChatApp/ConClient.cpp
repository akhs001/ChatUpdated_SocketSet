#include "ConClient.h"
#include <vector>
#include "Utils.h"

ConClient::ConClient()
{
	m_ip = { 0,0 };
	consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	//Initialize the socket
	m_socket = nullptr;
}

ConClient::~ConClient()
{
	ExitSession();
}


void ConClient::Receive(std::string& message , std::string& username , int& color)
{
	char response[2000];
	//receiving message

	if (SDLNet_TCP_Recv(m_socket, response, 2000) > 0)
	{
		//Save the string to a temp var
		std::string temp = response;
		//split the string
		std::vector<std::string> data = Utils::Split(temp, ':');
		//Now the data[0] is gonna be the Username
		//The data[1] is gonna be the message
		//Return the username
		username = data[0];
		//Return the message
		message = data[1];
		//Get color
		color =std::stoi( data[2]);
	}
}

bool ConClient::Send(const std::string& message)
{
	std::string  newMessage;

	newMessage = PrintUsername()  + ":" + message;

	int length = newMessage.length() + 1;
	//Senbd the message to the client

	if (SDLNet_TCP_Send(m_socket, newMessage.c_str(), length) > 0)
	{
		return true;
	}
	else
	{
		Utils::TypeWrite("Unfortunately you are disconected from the server", 50);
		OpenSocket();
		return false;
	}
	
}

void ConClient::sendThread()
{
	//Create the sending thread
	std::string Message = "";
	while (Message != "exit")
	{

		//Send Message
		std::getline(std::cin, Message);
;
		//Set the length of the message
		int length = Message.length() + 1;

			Send(Message);

	}
	//if you write exit
	ExitSession();
}

void ConClient::recvThread()
{
	//Create the Receive thread
	std::string Message = "";
	std::string Username = "";
	int color;

	while (Message != "exit")
	{
			Message = "";
			Username = "";
			color = 0;

			Receive(Message ,Username , color);

			if (Message != "" && Message != "exit")
			{
				//Set the color
				SetColor(color); 
				std::cout << Utils::space(10) << Username << ":" << Message << std::endl;	//Display the message
				
			}
			else if (Message == "exit")
			{
				CloseSocket();
			}
			else
			{
				continue;
			}
			SetColor(Color::DEFAULT);
	}
}

void ConClient::ExitSession()
{
	std::string Message = PrintUsername() + " leave the session";
	int length = Message.length() + 1;

		Send(Message);
		Utils::TypeWrite("Thanks for using MyChat.",50);
}

bool ConClient::OpenSocket()
{
	//Get the IP for connection
	char address[30];
	Utils::TypeWrite("Give the Server Ip Address to connect." ,50);
	std::cin >> address;
	Utils::clearConsole();

	//Setup the server and listening to the port 1234
	if (SDLNet_ResolveHost(&m_ip, address, 1234) == -1)
	{
		Utils::TypeWrite("mmmm.Unfortunately there was an...", 50);
		Utils::TypeWrite("Error Connecting to server" ,50);
		system("pause");
		return 0;
	}

	m_socket = SDLNet_TCP_Open(&m_ip);

	if (!m_socket)
	{
		Utils::TypeWrite("We have Problems connecting with the server right now",50);
		Utils::TypeWrite("Please Ensure the IP is correct and a server is running", 50);
		Utils::TypeWrite("at this IP", 50);
		OpenSocket();
	}

	Utils::TypeWrite( "Connected to Server.Lets Chat" ,50);
	//Send my Username to Server
	Send("$" + PrintUsername());
	system("pause");
	Utils::clearConsole();
}

void ConClient::SetColor(const int col)
{
	SetConsoleTextAttribute(consoleHandle, static_cast<WORD> (col));
}

void ConClient::SetColor(const Color col)
{
	SetConsoleTextAttribute(consoleHandle, static_cast<WORD> (col));
}

void ConClient::CloseSocket()
{
	SDLNet_TCP_Close(m_socket);
}
