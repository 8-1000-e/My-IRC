/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edubois- <edubois-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 13:41:07 by edubois-          #+#    #+#             */
/*   Updated: 2025/11/17 13:31:23 by edubois-         ###   ########.fr       */
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
#include <csignal>
#include <signal.h>
#include <arpa/inet.h>
#include <sstream>
#include <unistd.h> 
#include <sys/types.h>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include "Replies.hpp"
#include <string.h>
#include <cstdlib>
#include <utility>
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
        Server(Server const &src);
        
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
        Channel                     *GetChannel(std::string name);
        Server                      &operator=(Server const &src);
        Client                      *GetServerClientNick(std::string nickname);
        void                        SetFd(int fd);
        void                        SetPort(int port);
        void                        SetPassword(std::string password);
        std::string                 GetPassword();
        void                        AddClient(Client newClient);
        bool                        Server::notregistered(int fd);
        void                        AddChannel(Channel newChannel);
        void                        AddFds(pollfd newFd);
        std::vector<std::string>    splitCmd(std::string& cmd);
        void                        clientAuth(int fd, std::string cmd);
        bool                        isValidNickname(std::string& nickname);
        bool                        nickNameInUse(std::string& nickname);
        void                        setNickname(std::string cmd, int fd);
        void                        set_username(std::string& cmd, int fd);
        void	                    JOIN(std::string cmd, int fd);
	    int		                    SplitJoin(std::vector<std::pair<std::string, std::string> > &token, std::string cmd, int fd);
	    void	                    ExistCh(std::vector<std::pair<std::string, std::string> >&token, int i, int j, int fd);
	    void	                    NotExistCh(std::vector<std::pair<std::string, std::string> >&token, int i, int fd);
	    int		                    SearchForClients(std::string nickname);
	    void	                    PART(std::string cmd, int fd);
	    int		                    SplitCmdPart(std::string cmd, std::vector<std::string> &tmp, std::string &reason, int fd);
	    void	                    KICK(std::string cmd, int fd);
	    std::string                 SplitCmdKick(std::string cmd, std::vector<std::string> &tmp, std::string &user, int fd);
	    void	                    PRIVMSG(std::string cmd, int fd);
	    void	                    CheckForChannels_Clients(std::vector<std::string> &tmp, int fd);
	    void	                    QUIT(std::string cmd, int fd);
	    void 		                mode_command(std::string& cmd, int fd);
	    std::string                 invite_only(Channel *channel, char opera, std::string chain);
	    std::string                 topicRestriction(Channel *channel ,char opera, std::string chain);
	    std::string                 password_mode(std::vector<std::string> splited, Channel *channel, size_t &pos, char opera, int fd, std::string chain, std::string& arguments);
	    std::string                 operator_privilege(std::vector<std::string> splited, Channel *channel, size_t& pos, int fd, char opera, std::string chain, std::string& arguments);
	    std::string                 channel_limit(std::vector<std::string> splited, Channel *channel, size_t &pos, char opera, int fd, std::string chain, std::string& arguments);
	    bool		                isvalid_limit(std::string& limit);
	    std::string                 mode_toAppend(std::string chain, char opera, char mode);
	    std::vector<std::string>    splitParams(std::string params);
	    void                        getCmdArgs(std::string cmd,std::string& name, std::string& modeset ,std::string &params);
	    std::string                 tTopic();
	    void                        Topic(std::string &cmd, int &fd);
	    void                        Invite(std::string &cmd, int &fd);
	    std::string                 gettopic(std::string& input);
	    int                         getpos(std::string &cmd);

};

#endif