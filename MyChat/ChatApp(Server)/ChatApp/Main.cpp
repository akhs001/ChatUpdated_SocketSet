#include <iostream>
#include <SDL.h>
#include <SDL_net.h>
#include <string>
#include "ConServer.h"
#include <thread>
#include "Utils.h"


//Create the Server
ConServer server;

void chat()
{
	 std::thread listenThread(&ConServer::lisnThread , &server);
	 std::thread sendThread(&ConServer::sendThread , &server );
	 std::thread recvThread(&ConServer::recvThread , &server);

	if (listenThread.joinable())
		listenThread.join();
	
	if (sendThread.joinable())
		sendThread.join();

	if (recvThread.joinable())
		recvThread.join();
}

void GetUsername()
{
	std::string user = "";
	Utils::TypeWrite( "Choose a Username", 50);
	std::getline(std::cin, user);
	Utils::lineBreak(1);
	server.SetUsername(user);
	Utils::clearConsole();
	Utils::TypeWrite("Welcome to My Chat ", 50);
}

int main(int argc, char* argv[])
{
	//Draw the Ascii Logo
	Utils::draw();

	//Ask the user for a Name
	GetUsername();

	IPaddress& ip = server.GetIp();
	//Initialization
	if (!server.Initialize(1234 , ip ))
	{
		std::cout << "Error Initialize the app" << std::endl;
		system("pause");
		return 0;
	}

	if (!server.OpenSocket())
	{
		std::cout << "Cannot open the socket" << std::endl;
		system("pause");
		return 0;
	}
	Utils::TypeWrite("Waiting for Clients",50);
	Utils::TypeWrite("You can use -H command anytime to get Help", 50);

	//Start the chat Function
	chat();

	//After chat Close sockets and ShutDown Everything
	server.CloseSocket();
	server.ShutDown();

	
	system("pause");
	return 0;
}