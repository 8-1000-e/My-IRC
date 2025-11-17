/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edubois- <edubois-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 14:10:55 by edubois-          #+#    #+#             */
/*   Updated: 2025/11/17 13:10:41 by edubois-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

Server::Server() {this->ServerFdSocket = -1;}

Server::~Server(){}

Server::Server(Server const &src) {*this = src;}

Server &Server::operator=(Server const &src)
{
	if (this != &src)
	{
		this->Port = src.Port;
		this->ServerFdSocket = src.ServerFdSocket;
		this->password = src.password;
		this->clients = src.clients;
		this->channels = src.channels;
		this->fds = src.fds;
	}
	return *this;
}
        
int Server::GetPort()
{
    return this->Port;
}

int Server::GetFd()
{
    return this->ServerFdSocket;
}

Channel *Server::GetChannel(std::string name)
{
	for (size_t i = 0; i < this->channels.size(); i++)
	{
		if (this->channels[i].GetServerName() == name)
			return &channels[i];
	}
	return NULL;
}

Client *Server::GetServerClientNick(std::string nickname)
{
	for (size_t i = 0; i < this->clients.size(); i++){
		if (this->clients[i].GetNickName() == nickname)
			return &this->clients[i];
	}
	return NULL;
}

Client *Server::GetServerClient(int fd)
{
	for (size_t i = 0; i < this->clients.size(); i++)
    {
		if (this->clients[i].GetFd() == fd)
			return &this->clients[i];
	}
	return NULL;
}

void Server::SetFd(int fd){this->ServerFdSocket = fd;}

void Server::SetPort(int port){this->Port = port;}

void Server::SetPassword(std::string password){this->password = password;}

std::string Server::GetPassword(){return this->password;}

void Server::AddClient(Client newClient){this->clients.push_back(newClient);}

void Server::AddChannel(Channel newChannel){this->channels.push_back(newChannel);}

void Server::AddFds(pollfd newFd){this->fds.push_back(newFd);}

void Server::senderror(int code, std::string clientname, int fd, std::string msg)
{
	std::stringstream ss;
	ss << ":localhost " << code << " " << clientname << msg;
	std::string resp = ss.str();
	if(send(fd, resp.c_str(), resp.size(),0) == -1)
		std::cerr << "send() failed!" << std::endl;
}

void Server::senderror(int code, std::string clientname, std::string channelname, int fd, std::string msg)
{
	std::stringstream ss;
	ss << ":localhost " << code << " " << clientname << " " << channelname << msg;
	std::string resp = ss.str();
	if(send(fd, resp.c_str(), resp.size(),0) == -1)
		std::cerr << "send() failed!" << std::endl;
}

void Server::_sendResponse(std::string response, int fd)
{
	if(send(fd, response.c_str(), response.size(), 0) == -1)
		std::cerr << "Response send() failed!" << std::endl;
}

bool Server::StopSignal = false;

void Server::SignalHandler(int signum)
{
	(void)signum;
	std::cout << std::endl << "StopSignal Received!" << std::endl;
	Server::StopSignal = true;
}

void	Server::close_fds()
{
	for(size_t i = 0; i < clients.size(); i++)
    {
		std::cout << BOLDRED << "Client <" << clients[i].GetFd() << "> Disconnected" << RESET << std::endl;
		close(clients[i].GetFd());
	}
	if (ServerFdSocket != -1){	
		std::cout << BOLDRED << "Server <" << ServerFdSocket << "> Disconnected" << RESET << std::endl;
		close(ServerFdSocket);
	}
}

void    Server::ServerInit(int port, std::string password)
{
        this->password = password;
        this->Port = port;
        this->CreateServSocket();

        std::cout << BOLDCYAN << "   _____ __________ _    ____________ " << std::endl << "  / ___// ____/ __ \\ |  / / ____/ __ \\" << std::endl << "  \\__ \\/ __/ / /_/ / | / / __/ / /_/ /" << std::endl << " ___/ / /___/ _, _/| |/ / /___/ _, _/ " << std::endl << "/____/_____/_/ |_| |___/_____/_/ |_|  " << std::endl << RESET << std::endl << GREEN << RESET << "Socket FD: "  << BOLDYELLOW << ServerFdSocket << RESET << std::endl;
        std::cout << BOLDGREEN << "\n\n\nWaiting for connection..." << RESET << std::endl;
    
        while(!Server::StopSignal)
        {
       		if((poll(&fds[0],fds.size(),-1) == -1) && Server::StopSignal == false)
    			throw(std::runtime_error("poll() failed!\n"));
		    for (size_t i = 0; i < fds.size(); i++) 
            {
                if (fds[i].revents & POLLIN) //check for returned events
                {
       				if (fds[i].fd == ServerFdSocket)
    					this->AcceptNewClient();
    				else
    					this->ReceiveNewData(fds[i].fd);
                }
            }
            
        }
}
    
void    Server::CreateServSocket()
{
    int check = 1;
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_addr.s_addr = INADDR_ANY;
	ServerAddr.sin_port = htons(Port);
    //Create socket
	if ((ServerFdSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        throw(std::runtime_error("Failed to create socket!\n"));
        
    //bypass wait time to restart server with same socket
    if(setsockopt(ServerFdSocket, SOL_SOCKET, SO_REUSEADDR, &check, sizeof(check)) == -1)
		throw(std::runtime_error("Failed to set option \"fast reuse socket\" on socket!\n"));
    
    //put non-bloquant mod on FDSocket
	 if (fcntl(ServerFdSocket, F_SETFL, O_NONBLOCK) == -1)
		throw(std::runtime_error("Failed to set non-bloquant option  on socket!\n"));

    //lock socket with ip and adress
   	if (bind(ServerFdSocket, (struct sockaddr *)&ServerAddr, sizeof(ServerAddr)) == -1)
		throw(std::runtime_error("Failed to bind socket!\n"));
    
    //put socket in listen mod to accept connection
	if (listen(ServerFdSocket, SOMAXCONN) == -1)
		throw(std::runtime_error("Failed to put socket on listen mod!\n"));

    //add server socet to poll list to watch new connections
    NewClient.fd = ServerFdSocket;
    NewClient.events = POLLIN;
    NewClient.revents = 0;
    
    fds.push_back(NewClient);
}

void    Server::AcceptNewClient()
{
    Client client;
    socklen_t len = sizeof(ClientAdd);
    memset(&ClientAdd, 0, len);

    int IncomeFD = accept(ServerFdSocket, (sockaddr *)&(ClientAdd), &len);
    if (IncomeFD == -1)
    {
        std::cerr << "Can't accept() new Client!" << std::endl;
        return ;
    }
    NewClient.events = POLLIN;
    NewClient.revents = 0;
    NewClient.fd = IncomeFD;

    client.SetFd(IncomeFD);
    client.setIpAdd(inet_ntoa(ClientAdd.sin_addr));
    clients.push_back(client);
	fds.push_back(NewClient);

    std::cout << BOLDBLUE << IncomeFD << BOLDGREEN << "connected!" << RESET << std::endl;
}

void    Server::ReceiveNewData(int fd)
{
	std::vector<std::string> cmd;
	char buff[1024];
    memset(&buff, 0, sizeof(buff));
    Client *client = GetServerClient(fd);
    size_t bytes = recv(fd, buff, sizeof(buff) -1, 0);
   	if (bytes <= 0)
	{
		std::cout << RED << fd << "> Disconnected" << RESET << std::endl;
		RmChannels(fd);
		RemoveClient(fd);
		RemoveFds(fd);
		close(fd);
	}
    else
	{ 
		client->setBuffer(buff);
		if(client->getBuffer().find_first_of("\r\n") == std::string::npos)
			return;
		cmd = splitBuffer(client->getBuffer());
		for(size_t i = 0; i < cmd.size(); i++)
			this->parseCmd(cmd[i], fd);
		if(GetServerClient(fd))
			GetServerClient(fd)->clearBuffer();
	}
}

bool Server::notregistered(int fd)
{
	if (!GetServerClient(fd) || GetServerClient(fd)->GetNickName().empty() || GetServerClient(fd)->GetUserName().empty() || GetServerClient(fd)->GetNickName() == "*"  || !GetServerClient(fd)->GetLogedIn())
		return false;
	return true;
}

void Server::parseCmd(std::string &cmd, int fd)
{
	if(cmd.empty())
		return ;
	std::vector<std::string> splited_cmd = splitCmd(cmd);
	size_t found = cmd.find_first_not_of(" \t\v");
	if(found != std::string::npos)
		cmd = cmd.substr(found);
	if(splited_cmd.size() && (splited_cmd[0] == "BONG" || splited_cmd[0] == "bong"))
		return;
    if(splited_cmd.size() && (splited_cmd[0] == "PASS" || splited_cmd[0] == "pass"))
        clientAuth(fd, cmd);
	else if (splited_cmd.size() && (splited_cmd[0] == "NICK" || splited_cmd[0] == "nick"))
		setNickname(cmd,fd);
	else if(splited_cmd.size() && (splited_cmd[0] == "USER" || splited_cmd[0] == "user"))
		set_username(cmd, fd);
	else if (splited_cmd.size() && (splited_cmd[0] == "QUIT" || splited_cmd[0] == "quit"))
		QUIT(cmd,fd);
	else if(notregistered(fd))
	{
		if (splited_cmd.size() && (splited_cmd[0] == "KICK" || splited_cmd[0] == "kick"))
			KICK(cmd, fd);
		else if (splited_cmd.size() && (splited_cmd[0] == "JOIN" || splited_cmd[0] == "join"))
			JOIN(cmd, fd);
		else if (splited_cmd.size() && (splited_cmd[0] == "TOPIC" || splited_cmd[0] == "topic"))
			Topic(cmd, fd);
		else if (splited_cmd.size() && (splited_cmd[0] == "MODE" || splited_cmd[0] == "mode"))
			mode_command(cmd, fd);
		else if (splited_cmd.size() && (splited_cmd[0] == "PART" || splited_cmd[0] == "part"))
			PART(cmd, fd);
		else if (splited_cmd.size() && (splited_cmd[0] == "PRIVMSG" || splited_cmd[0] == "privmsg"))
			PRIVMSG(cmd, fd);
		else if (splited_cmd.size() && (splited_cmd[0] == "INVITE" || splited_cmd[0] == "invite"))
			Invite(cmd,fd);
		else if (splited_cmd.size())
			_sendResponse(ERR_CMDNOTFOUND(GetServerClient(fd)->GetNickName(),splited_cmd[0]),fd);
	}
	else if (!notregistered(fd))
		_sendResponse(ERR_NOTREGISTERED(std::string("*")),fd);
}

std::vector<std::string> Server::splitBuffer(std::string str)
{
	std::vector<std::string> vec;
	std::istringstream stm(str);
	std::string line;
	while(std::getline(stm, line))
	{
		size_t pos = line.find_first_of("\r\n");
		if(pos != std::string::npos)
			line = line.substr(0, pos);
		vec.push_back(line);
	}
	return vec;
}

std::vector<std::string> Server::splitCmd(std::string& cmd)
{
	std::vector<std::string> vec;
	std::istringstream stm(cmd);
	std::string token;
	while(stm >> token)
	{
		vec.push_back(token);
		token.clear();
	}
	return vec;
}

void	Server::RmChannels(int fd)
{
	for (size_t i = 0; i < this->channels.size(); i++)
    {
		int flag = 0;
		if (channels[i].GetChannelClient(fd))
		{
            channels[i].remove_client(fd);
            flag = 1;
        }
		else if (channels[i].get_admin(fd))
		{
            channels[i].remove_admin(fd);
            flag = 1;
        }
		if (!channels[i].GetServerClientsNumber())
		{
            channels.erase(channels.begin() + i);
            i--;
            continue;
        }
		if (flag)
        {
			std::string rpl = ":" + GetServerClient(fd)->GetNickName() + "!~" + GetServerClient(fd)->GetUserName() + "@localhost QUIT Quit\r\n";
			channels[i].sendToAll(rpl);
		}
	}
}

void Server::RemoveChannel(std::string name)
{
	for (size_t i = 0; i < this->channels.size(); i++)
    {
		if (this->channels[i].GetServerName() == name)
		{
            this->channels.erase(this->channels.begin() + i);
            return;
        }
	}
}

void Server::RemoveFds(int fd)
{
	for (size_t i = 0; i < this->fds.size(); i++)
    {
		if (this->fds[i].fd == fd)
        {
            this->fds.erase(this->fds.begin() + i);
            return;
        }
	}
}

void Server::RemoveClient(int fd)
{
	for (size_t i = 0; i < this->clients.size(); i++)
    {
		if (this->clients[i].GetFd() == fd)
		{
            this->clients.erase(this->clients.begin() + i);
            return;
        }
	}
}
