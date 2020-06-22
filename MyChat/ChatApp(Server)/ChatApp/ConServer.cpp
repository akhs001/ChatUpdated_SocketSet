#include "ConServer.h"
#include <SDL_net.h>
#include "Utils.h"


ConServer::ConServer()
{
	m_socketSet = SDLNet_AllocSocketSet(MAX_CLIENTS);
	consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	//Initialize the sockets
	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		m_socket[i] = nullptr;

	}
	m_color = 1;
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
	SDLNet_TCP_AddSocket(m_socketSet, m_socket[socketToUse]);
	std::cout << GetClientName(socketToUse) << " connected" << std::endl;
	//Send welcome message to client
	Send(m_welcomeMessage, "", socketToUse, 0);
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

void ConServer::PrintHelp()
{
	Utils::clearConsole();
	SetColor(Color::DEFAULT);
	Utils::Log("********Chat Commands*********");
	Utils::Log("-H          ->            Help");
	Utils::Log("-U          ->     Client List");
	Utils::Log("-N*NAME     ->     Change name");
	Utils::Log("-X			->   Clear Console");
	Utils::Log("-W*MSG      ->  Change Welcome");
	Utils::Log("-C*ID       ->    Change Color");
	Utils::Log("-K*ID       ->     Kick Client");
	Utils::Log("******************************");
	Utils::Log("* is space.");
	Utils::Log("MSG is the new welcome message");
	Utils::Log("ID is integer");
	Utils::Log("*********Color Codes**********");
	//Print color codes
	for (int i = 1; i < 15; i++)
	{
		SetColor(i);
		Utils::Log(std::to_string( i) + " My Chat");
	}
	Utils::Log("******************************");
}

const int ConServer::GetMaxClients()
{
	return MAX_CLIENTS;
}

bool ConServer::Send(const std::string& message,const std::string sender,const int index , int from)
{
	std::string  newMessage;
	//Add name to message
	if (sender == "")
	{
		//If sender is "" means that the message is from server
		newMessage = PrintUsername() + ":" + message + ":" + std::to_string(1);
	}
	else
	{
		//Else the sender is from Another Client so we put his name here
		newMessage = sender + ":" + message + ":" + std::to_string(from+2);
	}
	

	int length = newMessage.length() + 1;
	//Senbd the message to the client

	if (m_socket[index] && SDLNet_TCP_Send(m_socket[index], newMessage.c_str(), length) > length)
	{
		return true;
	}

	return false;

}

//add username to the map
void ConServer::AddUsername(std::string& username , int& socket)
{
	m_ClientNames[socket] = username;
}

std::string ConServer::GetClientName(int socket)
{
	return m_ClientNames[socket];
}

int ConServer::GetClientSize()
{
	return m_ClientNames.size();
}

void ConServer::RemoveClientName(int socket)
{
	m_ClientNames.erase(socket);
}

void ConServer::Receive(std::string& message, std::string& name , int index)
{
	char response[2000];
	//receiving message

	if ( SDLNet_TCP_Recv(m_socket[index], response, 2000) > 0)
	{
		//Save the string to a temp var
		std::string temp = response;
		//split the string
		std::vector<std::string> data = Utils::Split(temp, ':');
		//Now the data[0] is gonna be the Username
		//The data[1] is gonna be the message
		//Return the username
		name = data[0];
		//Return the message
		message = data[1];

		//If the first char is $ mean that the client is sending his name to server
		//So this is not actually a message
		if (message[0] == '$')
		{
			//But we have to store the username
			std::string name = message.substr(1, message.length());
			//Save the username to my ClientNames map, index is the user socket
			AddUsername( name , index);
			//Finally delete the message so it will no displayed
			message = "";
		}
	}
	else 	//User Lost connection
	{
		if (GetClientName(index) != "")
		{
			Utils::TypeWrite(GetClientName(index) + " has left the chat.", 50);
			//Remove the name from the Map
			RemoveClientName(index);
			//Close the socket
			CloseSocket(index);
		}
	
	}
}


void ConServer::CloseSocket()
{
	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		SDLNet_TCP_Close(m_socket[i]);
		SDLNet_TCP_DelSocket(m_socketSet, m_socket[i]);
	}
	SDLNet_TCP_Close(m_listenSocket);
}



void ConServer::sendThread()
{
	int UserToKick;
	//Create the sending thread
		std::string Message = "";
		while (Message != "exit")
		{
			SetColor(m_color);
			//Send Message
			std::getline(std::cin, Message);

			//Check if the message is Not a message but a  server command
			//I would like to create server command to do some staff
			//For Example my first command is to change the welcome message
			//Every command sould start with "-" char
			//So you can change the Welcome message if you write something like
			// -W "Welcome message"

			//First check if the first char is '-'
			if (Message[0] == '-')
			{
				//We have a command
				//Check what command we have By checking the second char
				char cmd = Message[1];

				switch (cmd)
				{
				case 'H':
					//H Print a list with all available commands
					PrintHelp();
					break;
				case 'U':
					//U Print a List with the Client Names nad the socket Used from each one
					Utils::Log("***Current Chat Clients***");
					for (int i = 0; i < GetClientSize(); i++)
					{
						Utils::Log(std::to_string( i) + "  ->  " + GetClientName(i));
					}
					Utils::Log("**************************");
					break;
				case 'N':
					//N Changing the Server Name
					SetUsername ( Message.substr(3, Message.length()));
					Utils::TypeWrite("Server name has changed to " + PrintUsername(),50 );
					break;
				case 'X':
					//X is cleaning the console
					system("cls");
					Utils::draw();
					break;
				case 'W':
					//W is for change the welcome message
					m_welcomeMessage = Message.substr(3, Message.length());
					Utils::TypeWrite("Welcome message has changed",50);
					break;
				case 'C':
					m_color =std::stoi( Message.substr(3, Message.length()));
					Utils::TypeWrite("Server Color has changed",50);
					break;
				case 'K':
					//With K you can kick someone from the chat You have to pass Socket number and NOT client name here
					//To find user socket use the -U  command
					UserToKick = std::stoi(Message.substr(3, Message.length()));
					//Send the Kick message to Client
					Send("Server kicked you from the chat ", "", UserToKick, 0);
					Utils::TypeWrite("You kick " + GetClientName(UserToKick) + " from chat",50);
					//Remove the name from the Map
					RemoveClientName(UserToKick);
					//Close the socket for this client so he cannon recieve or send messages
					CloseSocket(UserToKick);
					break;
				 default:
					 Utils::TypeWrite("Command not found.Use -H for help",50);
				}
			}
			else
			{
				//Set the length of the message
				int length = Message.length() + 1;

				for (int i = 0; i < GetMaxClients(); i++)
				{
					Send(Message, "", i, 0);
				}
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
		int color = 0;

		while (true)
		{
			if (GetClientSize() > 0)
			{
				//Check for Socket that are ready
				int numSocketsReady = SDLNet_CheckSockets(m_socketSet, 50);
				//If we have message
				if (numSocketsReady > 0)
				{
					for (int i = 0; i < MAX_CLIENTS; i++)
					{
						if (m_socket[i] == nullptr) { continue; }
						//Initialize the message from each client
						Message = "";
						Username = "";
						if (SDLNet_SocketReady(m_socket[i]))
						{
							//Call receive and retreive Message and username
							Receive(Message, Username, i);

							if (Message != "" && Message != "exit")
							{
								//If we receive a message from a Client
								//First we need to Display to server
								SetColor(i + 2);
								std::cout << "           " << Username << ":" << Message << std::endl;	//Display the message
								SetColor(Color::DEFAULT);
								//And then Forward the message  to everyone else Client
								ForwardMessage(Message, Username, i);
							}
							else if (Message == "exit")
							{
								//If the client send exit we need to log him out.
								if (Username != "")
								{
									Utils::TypeWrite(Username + " has leave the chat", 50);
									//Remove the name from the Map
									RemoveClientName(i);
									//Close the socket
									CloseSocket(i);
								}


							}
							else
							{
								//If nothing from above then continue with other clients
								continue;
							}

						}




					}
				}
			}
		}
	}

void ConServer::lisnThread()
{
		//If We dont have more free Sockets stop Listening
		while (GetFreeSocket() != -1)
		{
		ListenSocket();
		}
	
}

//This function gets a message fro a client and forward it to all other clients
void ConServer::ForwardMessage(const std::string message,const std::string name, const int fromClient)
{
	//Forward a client message to All other ClientsI create 

		for (int i = 0; i < GetMaxClients(); i++)
		{
			if (i != fromClient)
			Send(message, name, i , fromClient);
		}
}

//This function sets the color of the text
void ConServer::SetColor(const Color col)
{
	SetConsoleTextAttribute(consoleHandle, static_cast<WORD> (col));
}
//This function sets the color of the text
void ConServer::SetColor(const int col)
{
	SetConsoleTextAttribute(consoleHandle, static_cast<WORD> (col));
}

void ConServer::CloseSocket(const int socketNumber)
{
	SDLNet_TCP_Close(m_socket[socketNumber]);
	SDLNet_TCP_DelSocket(m_socketSet, m_socket[socketNumber]);
	m_socket[socketNumber] = nullptr;
}


//Open the Listen Socket and wait for Clients to connect
bool ConServer::OpenSocket()
{
	m_listenSocket = SDLNet_TCP_Open(&GetIp() );

	if (!m_listenSocket)
	{
		Utils::TypeWrite("Could not open Listen socket for connection" ,50);
		system("pause");
		return false;
	}
	return true;
}

//This called when server is off.
void ConServer::ExitSession()
{
	std::string Message = PrintUsername() + " leave the session";
	int length = Message.length() + 1;

	//Send The message to everyone
	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		Send(Message , "" , i, 0 );
	}
	

}
