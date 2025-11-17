/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JOIN.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edubois- <edubois-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 13:04:14 by edubois-          #+#    #+#             */
/*   Updated: 2025/11/17 13:04:15 by edubois-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

int Server::SplitJoin(std::vector<std::pair<std::string, std::string> > &token, std::string cmd, int fd)
{
	std::vector<std::string> tmp;
	std::string ChStr, PassStr, buff;
	std::istringstream iss(cmd);
	while(iss >> cmd)
		tmp.push_back(cmd);
	if (tmp.size() < 2) {token.clear(); return 0;}
	tmp.erase(tmp.begin());
	ChStr = tmp[0]; tmp.erase(tmp.begin());
	if (!tmp.empty()) {PassStr = tmp[0]; tmp.clear();}
	for (size_t i = 0; i < ChStr.size(); i++){
		if (ChStr[i] == ',')
				{token.push_back(std::make_pair(buff, "")); buff.clear();}
		else buff += ChStr[i];
	}
	token.push_back(std::make_pair(buff, ""));
	if (!PassStr.empty()){
		size_t j = 0; buff.clear();
		for (size_t i = 0; i < PassStr.size(); i++){
			if (PassStr[i] == ',')
				{token[j].second = buff; j++; buff.clear();}
			else buff += PassStr[i];
		}
		token[j].second = buff;
	}
	for (size_t i = 0; i < token.size(); i++)//erase the empty channel names
		{if (token[i].first.empty())token.erase(token.begin() + i--);}
	for (size_t i = 0; i < token.size(); i++){//ERR_NOSUCHCHANNEL (403) // if the channel doesn't exist
		if (*(token[i].first.begin()) != '#')
			{senderror(403, GetServerClient(fd)->GetNickName(), token[i].first, GetServerClient(fd)->GetFd(), " :No such channel\r\n"); token.erase(token.begin() + i--);}
		else
			token[i].first.erase(token[i].first.begin());
	}
	return 1;
}

int Server::SearchForClients(std::string nickname)
{
	int count = 0;
	for (size_t i = 0; i < this->channels.size(); i++){
		if (this->channels[i].GetServerClientInChannel(nickname))
			count++;
	}
	return count;
}

bool IsInvited(Client *cli, std::string ChName, int flag){
	if(cli->GetInviteChannel(ChName)){
		if (flag == 1)
			cli->RmChannelInvite(ChName);
		return true;
	}
	return false;
}

void Server::ExistCh(std::vector<std::pair<std::string, std::string> >&token, int i, int j, int fd)
{
	if (this->channels[j].GetServerClientInChannel(GetServerClient(fd)->GetNickName()))// if the client is already registered
		return;
	if (SearchForClients(GetServerClient(fd)->GetNickName()) >= 10)//ERR_TOOMANYCHANNELS (405) // if the client is already in 10 channels
		{senderror(405, GetServerClient(fd)->GetNickName(), GetServerClient(fd)->GetFd(), " :You have joined too many channels\r\n"); return;}
	if (!this->channels[j].GetPassword().empty() && this->channels[j].GetPassword() != token[i].second){// ERR_BADCHANNELKEY (475) // if the password is incorrect
		if (!IsInvited(GetServerClient(fd), token[i].first, 0))
			{senderror(475, GetServerClient(fd)->GetNickName(), "#" + token[i].first, GetServerClient(fd)->GetFd(), " :Cannot join channel (+k) - bad key\r\n"); return;}
	}
	if (this->channels[j].GetInvitOnly()){// ERR_INVITEONLYCHAN (473) // if the channel is invit only
		if (!IsInvited(GetServerClient(fd), token[i].first, 1))
			{senderror(473, GetServerClient(fd)->GetNickName(), "#" + token[i].first, GetServerClient(fd)->GetFd(), " :Cannot join channel (+i)\r\n"); return;}
	}
	if (this->channels[j].GetLimit() && this->channels[j].GetServerClientsNumber() >= this->channels[j].GetLimit())// ERR_CHANNELISFULL (471) // if the channel reached the limit of number of clients
		{senderror(471, GetServerClient(fd)->GetNickName(), "#" + token[i].first, GetServerClient(fd)->GetFd(), " :Cannot join channel (+l)\r\n"); return;}
	// add the client to the channel
	Client *cli = GetServerClient(fd);
	this->channels[j].add_client(*cli);
	if(channels[j].GetTopicName().empty())
		_sendResponse(RPL_JOINMSG(GetServerClient(fd)->getHostname(),GetServerClient(fd)->getIpAdd(),token[i].first) + \
			RPL_NAMREPLY(GetServerClient(fd)->GetNickName(),channels[j].GetServerName(),channels[j].clientChannel_list()) + \
			RPL_ENDOFNAMES(GetServerClient(fd)->GetNickName(),channels[j].GetServerName()),fd);
	else
		_sendResponse(RPL_JOINMSG(GetServerClient(fd)->getHostname(),GetServerClient(fd)->getIpAdd(),token[i].first) + \
			RPL_TOPICIS(GetServerClient(fd)->GetNickName(),channels[j].GetServerName(),channels[j].GetTopicName()) + \
			RPL_NAMREPLY(GetServerClient(fd)->GetNickName(),channels[j].GetServerName(),channels[j].clientChannel_list()) + \
			RPL_ENDOFNAMES(GetServerClient(fd)->GetNickName(),channels[j].GetServerName()),fd);
    channels[j].sendToAll(RPL_JOINMSG(GetServerClient(fd)->getHostname(),GetServerClient(fd)->getIpAdd(),token[i].first), fd);
}


void Server::NotExistCh(std::vector<std::pair<std::string, std::string> >&token, int i, int fd)
{
	if (SearchForClients(GetServerClient(fd)->GetNickName()) >= 10)//ERR_TOOMANYCHANNELS (405) // if the client is already in 10 channels
		{senderror(405, GetServerClient(fd)->GetNickName(), GetServerClient(fd)->GetFd(), " :You have joined too many channels\r\n"); return;}
	Channel newChannel;
	newChannel.SetName(token[i].first);
	newChannel.add_admin(*GetServerClient(fd));
	newChannel.setCreationTime();
	this->channels.push_back(newChannel);
	// notifiy thet the client joined the channel
    _sendResponse(RPL_JOINMSG(GetServerClient(fd)->getHostname(),GetServerClient(fd)->getIpAdd(),newChannel.GetServerName()) + \
        RPL_NAMREPLY(GetServerClient(fd)->GetNickName(),newChannel.GetServerName(),newChannel.clientChannel_list()) + \
        RPL_ENDOFNAMES(GetServerClient(fd)->GetNickName(),newChannel.GetServerName()),fd);
}

void Server::JOIN(std::string cmd, int fd)
{
	std::vector<std::pair<std::string, std::string> > token;
	// SplitJoin(token, cmd, fd);
	if (!SplitJoin(token, cmd, fd))// ERR_NEEDMOREPARAMS (461) // if the channel name is empty
		{senderror(461, GetServerClient(fd)->GetNickName(), GetServerClient(fd)->GetFd(), " :Not enough parameters\r\n"); return;}
	if (token.size() > 10) //ERR_TOOMANYTARGETS (407) // if more than 10 Channels
		{senderror(407, GetServerClient(fd)->GetNickName(), GetServerClient(fd)->GetFd(), " :Too many channels\r\n"); return;}
	for (size_t i = 0; i < token.size(); i++){
		bool flag = false;
		for (size_t j = 0; j < this->channels.size(); j++){
			if (this->channels[j].GetServerName() == token[i].first){
				ExistCh(token, i, j, fd);
				flag = true; break;
			}
		}
		if (!flag)
			NotExistCh(token, i, fd);
	}
}