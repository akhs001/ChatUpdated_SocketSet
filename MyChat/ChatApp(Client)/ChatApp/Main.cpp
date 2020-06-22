#include <iostream>
#include <SDL.h>
#include <SDL_net.h>
#include <string>
#include "TCPConnection.h"
#include <Windows.h>
#include <thread>
#include "ConClient.h"
#include "Utils.h"

//Boolean to determine if the chat is running
bool isRunning = true;

//Create the Client Object
ConClient client;


//The Chat function responsible for sending and receiving messages
void chat()
{
	std::thread sendThread;
	std::thread recvThread;


	sendThread = std::thread(&ConClient::sendThread , &client);
	recvThread = std::thread(&ConClient::recvThread , &client);

	if (sendThread.joinable())
		sendThread.join();

	if (recvThread.joinable())
		recvThread.join();
}




//Ask the user for a Username
void GetUsername()
{
	std::string user = "";
	Utils::TypeWrite("Choose a Username", 50);
	std::getline(std::cin, user);
	Utils::lineBreak(1);
	client.SetUsername(user);
	Utils::clearConsole();
	Utils::TypeWrite("Welcome to My Chat " + user , 50);

}


int main(int argc , char* argv[])
{
	//Draw the Logo
	Utils:: draw();
	//Ask for Username
	GetUsername();

	//Initialization
	if (!client.Initialize(1234))
	{
		Utils::Log( "Error Initialize the app" );
		system("pause");
		return 0;
	}

	if (!client.OpenSocket())
	{
		Utils::Log("Cannot open the socket");
		system("pause");
		return 0;
	}

	//Chat
	chat();

	//Finish
	isRunning = false;

	//ShutDown and Close Sockets
	client.CloseSocket();
	client.ShutDown();


	system("pause");
	return 0;
}