#pragma once
#include <string>
#include <SDL.h>
#include <SDL_net.h>
#include <vector>



class TCPConnection
{
public:
	TCPConnection();

public :
	bool Initialize(Uint16 port, IPaddress& ip);

	IPaddress& GetIp();
	std::string PrintUsername();
	void SetUsername(std::string username);

	virtual bool OpenSocket() ;
	virtual bool Send(std::string& message, std::string sender, int index) ;
	virtual bool Send(std::string& message) ;

	virtual void Receive(std::string& message, std::string& name, int index) {};
	virtual void Receive(std::string& message) {};

	virtual void CloseSocket() {};

	void ShutDown();

private:
	IPaddress m_IP;
	std::string m_username;

};

