/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edubois- <edubois-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 17:51:46 by edubois-          #+#    #+#             */
/*   Updated: 2025/11/17 11:34:21 by edubois-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include "Client.hpp"
#include "Server.hpp"
#include <ctime>
class Client;
class Server;

class Channel
{
    private:
    	int invitOnly;
	    int topic;
	    int key;
	    int limit;
	    bool topicRestriction;
	    std::string name;
	    std::string timeCreation;
	    std::string password;
	    std::string created_at;
	    std::string topicName;
    	std::vector<Client> clients;
	    std::vector<Client> admins;
	    std::vector<std::pair<char, bool> > modes;
        
    public:
        Channel();
        ~Channel();
        Channel(Channel const &src);
    	Channel &operator=(Channel const &src);

        void SetInvitOnly(int invitOnly);
        void SetTopic(int topic);
        void SetTime(std::string time);
        void SetKey(int key);
        void SetLimit(int limit);
        void SetTopicName(std::string topicName);
        void SetPassword(std::string password);
        void SetName(std::string name);
        void set_topicRestriction(bool value);
        void setModeAtindex(size_t index, bool mode);
        void setCreationTime();
        std::string GetTopicName();
        std::string GetPassword();
        std::string GetServerName();
        std::string GetTime();
        std::string get_creationtime();
        std::string getModes();
        std::string clientChannel_list();
        Client *GetChannelClient(int fd);
        Client *get_admin(int fd);
        Client* GetServerClientInChannel(std::string name);
        int GetInvitOnly();
        int GetTopic() ;
        int GetKey() ;
        int GetLimit();
        int GetServerClientsNumber();
        bool GettopicRestriction() const;
        bool getModeAtindex(size_t index);
        bool clientInChannel(std::string &nick);
        void add_client(Client newClient);
        void add_admin(Client newClient);
        void remove_client(int fd);
        void remove_admin(int fd);
        bool changeClientToAdmin(std::string& nick);
        bool changeAdminToClient(std::string& nick);
        void sendToAll(std::string rpl1);
        void sendToAll(std::string rpl1, int fd);

};

#endif