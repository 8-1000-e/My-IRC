/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edubois- <edubois-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 13:41:07 by edubois-          #+#    #+#             */
/*   Updated: 2025/11/16 18:55:11 by edubois-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <vector>
#include "Client.hpp"
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <arpa/inet.h>
#include <sstream>
#include <unistd.h> 
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "Channel.hpp"

// texte color
#define RESET   "\033[0m"

#define BLACK   "\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

// bold text color
#define BOLDBLACK   "\033[1m\033[30m"
#define BOLDRED     "\033[1m\033[31m"
#define BOLDGREEN   "\033[1m\033[32m"
#define BOLDYELLOW  "\033[1m\033[33m"
#define BOLDBLUE    "\033[1m\033[34m"
#define BOLDMAGENTA "\033[1m\033[35m"
#define BOLDCYAN    "\033[1m\033[36m"
#define BOLDWHITE   "\033[1m\033[37m"

#define auto __auto_type

class Channel;
class Client;

class Server
{
    private:
        int Port;
        std::string password;
        int ServerFdSocket;
        static bool StopSignal;
        struct pollfd NewClient;
    	std::vector<struct pollfd> fds;
        struct sockaddr_in ServerAddr;
        struct sockaddr_in ClientAdd;
    	std::vector<Client> clients;
        std::vector<Channel> channels;
        
    public:
        Server();
        ~Server();
        
        void    ServerInit(int port, std::string password);
        void    CreateServSocket();
        void    AcceptNewClient();
        void    ReceiveNewData(int fd);

        static void signalHandler(int signum);
            
        void                        CloseFds();
        void                        ClearClient();
        void	                    RmChannels(int fd);
        void                        RemoveChannel(std::string name);
        void                        RemoveFds(int fd);
        void                        RemoveClient(int fd);
        std::vector<std::string>    splitBuffer(std::string str);
        int                         GetPort();
        int                         GetFd();
        Client                      *GetServerClient(int fd);
        void                        parseCmd(std::string &cmd, int fd);
        void                        senderror(int code, std::string clientname, int fd, std::string msg);
        void                        senderror(int code, std::string clientname, std::string channelname, int fd, std::string msg);
        void                        _sendResponse(std::string response, int fd);
        void                        SignalHandler(int signum);
        void	                    close_fds();

};

#endif