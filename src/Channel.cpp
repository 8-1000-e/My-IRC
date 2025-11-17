/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edubois- <edubois-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 10:39:00 by edubois-          #+#    #+#             */
/*   Updated: 2025/11/17 11:22:27 by edubois-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"

Channel::Channel()
{
	this->invitOnly = 0;
	this->topic = 0;
	this->key = 0;
	this->limit = 0;
	this->topicRestriction = false;
	this->name = "";
	this->topicName = "";
	char charaters[5] = {'i', 't', 'k', 'o', 'l'};
	for(int i = 0; i < 5; i++)
		modes.push_back(std::make_pair(charaters[i],false));
	this->created_at = "";
}

Channel::~Channel() {}

Channel::Channel(Channel const &src) {*this = src;}

Channel &Channel::operator=(Channel const &src)
{
	if (this != &src)
    {
		this->invitOnly = src.invitOnly;
		this->topic = src.topic;
		this->key = src.key;
		this->limit = src.limit;
		this->topicRestriction = src.topicRestriction;
		this->name = src.name;
		this->password = src.password;
		this->created_at = src.created_at;
		this->topicName = src.topicName;
		this->clients = src.clients;
		this->admins = src.admins;
		this->modes = src.modes;
	}
	return *this;
}

void Channel::SetInvitOnly(int invitOnly)
{
    this->invitOnly = invitOnly;
}

void Channel::SetTopic(int topic)
{
    this->topic = topic;
}

void Channel::SetTime(std::string time)
{
    this->timeCreation = time;
}

void Channel::SetKey(int key)
{
    this->key = key;
}

void Channel::SetLimit(int limit)
{
    this->limit = limit;
}

void Channel::SetTopicName(std::string topicName)
{
    this->topicName = topicName;
}

void Channel::SetPassword(std::string password)
{
    this->password = password;
}

void Channel::SetName(std::string name)
{
    this->name = name;
}

void Channel::set_topicRestriction(bool value)
{
    this->topicRestriction = value;
}

void Channel::setModeAtindex(size_t index, bool mode)
{
    modes[index].second = mode;
}

void Channel::setCreationTime()
{
	std::time_t _time = std::time(NULL);
	std::ostringstream oss;
	oss << _time;
	this->created_at = std::string(oss.str());
}

int Channel::GetInvitOnly() {return this->invitOnly;}

int Channel::GetTopic() {return this->topic;}

int Channel::GetKey() {return this->key;}

int Channel::GetLimit() {return this->limit;}

int Channel::GetServerClientsNumber() {return this->clients.size() + this->admins.size();}

bool Channel::GettopicRestriction() const {return this->topicRestriction;}

bool Channel::getModeAtindex(size_t index) {return modes[index].second;}

bool Channel::clientInChannel(std::string &nick)
{
    
	for(size_t i = 0; i < clients.size(); i++)
    {
		if(clients[i].GetNickName() == nick)
			return true;
	}
	for(size_t i = 0; i < admins.size(); i++)
    {
		if(admins[i].GetNickName() == nick)
			return true;
	}
	return false;
}
std::string Channel::GetTopicName() {return this->topicName;}

std::string Channel::GetPassword() {return this->password;}

std::string Channel::GetServerName() {return this->name;}

std::string Channel::GetTime() {return this->timeCreation;}

std::string Channel::get_creationtime() {return created_at;}

std::string Channel::getModes()
{
	std::string mode;
	for(size_t i = 0; i < modes.size(); i++)
    {
		if(modes[i].first != 'o' && modes[i].second)
			mode.push_back(modes[i].first);
	}
	if(!mode.empty())
		mode.insert(mode.begin(),'+');
	return mode;
}

std::string Channel::clientChannel_list()
{
	std::string list;
	for(size_t i = 0; i < admins.size(); i++)
    {
		list += "@" + admins[i].GetNickName();
		if((i + 1) < admins.size())
			list += " ";
	}
	if(clients.size())
		list += " ";
	for(size_t i = 0; i < clients.size(); i++)
    {
		list += clients[i].GetNickName();
		if((i + 1) < clients.size())
			list += " ";
	}
	return list;
}

Client *Channel::GetChannelClient(int fd)
{
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
    {
		if (it->GetFd() == fd)
			return &(*it);
	}
	return NULL;
}

Client *Channel::get_admin(int fd)
{
	for (std::vector<Client>::iterator it = admins.begin(); it != admins.end(); ++it)
    {
		if (it->GetFd() == fd)
			return &(*it);
	}
	return NULL;
}

Client* Channel::GetServerClientInChannel(std::string name)
{
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
    {
		if (it->GetNickName() == name)
			return &(*it);
	}
	for (std::vector<Client>::iterator it = admins.begin(); it != admins.end(); ++it)
    {
		if (it->GetNickName() == name)
			return &(*it);
	}
	return NULL;
}

void Channel::add_client(Client newClient) {clients.push_back(newClient);}

void Channel::add_admin(Client newClient) {admins.push_back(newClient);}

void Channel::remove_client(int fd)
{
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
    {
		if (it->GetFd() == fd)
			{clients.erase(it); break;}
	}
}
void Channel::remove_admin(int fd)
{
	for (std::vector<Client>::iterator it = admins.begin(); it != admins.end(); ++it)
    {
		if (it->GetFd() == fd)
			{admins.erase(it); break;}
	}
}
bool Channel::changeClientToAdmin(std::string& nick)
{
	size_t i = 0;
	for(; i < clients.size(); i++)
    {
		if(clients[i].GetNickName() == nick)
			break;
	}
	if(i < clients.size())
    {
		admins.push_back(clients[i]);
		clients.erase(i + clients.begin());
		return true;
	}
	return false;
}

bool Channel::changeAdminToClient(std::string& nick)
{
	size_t i = 0;
	for(; i < admins.size(); i++)
    {
		if(admins[i].GetNickName() == nick)
			break;
	}
	if(i < admins.size())
    {
		clients.push_back(admins[i]);
		admins.erase(i + admins.begin());
		return true;
	}
	return false;

}
void Channel::sendToAll(std::string rpl1)
{
	for(size_t i = 0; i < admins.size(); i++)
		if(send(admins[i].GetFd(), rpl1.c_str(), rpl1.size(),0) == -1)
			std::cerr << "send() failed!" << std::endl;
	for(size_t i = 0; i < clients.size(); i++)
		if(send(clients[i].GetFd(), rpl1.c_str(), rpl1.size(),0) == -1)
			std::cerr << "send() failed!" << std::endl;
}
void Channel::sendToAll(std::string rpl1, int fd)
{
	for(size_t i = 0; i < admins.size(); i++)
    {
		if(admins[i].GetFd() != fd)
			if(send(admins[i].GetFd(), rpl1.c_str(), rpl1.size(),0) == -1)
				std::cerr << "send() failed!" << std::endl;
	}
	for(size_t i = 0; i < clients.size(); i++)
    {
		if(clients[i].GetFd() != fd)
			if(send(clients[i].GetFd(), rpl1.c_str(), rpl1.size(),0) == -1)
				std::cerr << "send() failed!" << std::endl;
	}
}