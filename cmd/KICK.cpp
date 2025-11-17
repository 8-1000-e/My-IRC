/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edubois- <edubois-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 13:04:17 by edubois-          #+#    #+#             */
/*   Updated: 2025/11/17 13:04:18 by edubois-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
void FindK(std::string cmd, std::string tofind, std::string &str)
{
	size_t i = 0;
	for (; i < cmd.size(); i++){
		if (cmd[i] != ' '){
			std::string tmp;
			for (; i < cmd.size() && cmd[i] != ' '; i++)
				tmp += cmd[i];
			if (tmp == tofind) break;
			else tmp.clear();
		}
	}
	if (i < cmd.size()) str = cmd.substr(i);
	i = 0;
	for (; i < str.size() && str[i] == ' '; i++);
	str = str.substr(i);
}

std::string SplitCmdK(std::string &cmd, std::vector<std::string> &tmp)
{
	std::stringstream ss(cmd);
	std::string str, reason;
	int count = 3;
	while (ss >> str && count--)
		tmp.push_back(str);
	if(tmp.size() != 3) return std::string("");
	FindK(cmd, tmp[2], reason);
	return reason;
}

std::string Server::SplitCmdKick(std::string cmd, std::vector<std::string> &tmp, std::string &user, int fd)
{
	std::string reason = SplitCmdK(cmd, tmp);
	if (tmp.size() < 3) // check if the client send the channel name and the client to kick
		return std::string("");
	tmp.erase(tmp.begin());
	std::string str = tmp[0]; std::string str1;
	user = tmp[1]; tmp.clear();
	for (size_t i = 0; i < str.size(); i++){//split the first string by ',' to get the channels names
		if (str[i] == ',')
			{tmp.push_back(str1); str1.clear();}
		else str1 += str[i];
	}
	tmp.push_back(str1);
	for (size_t i = 0; i < tmp.size(); i++)//erase the empty strings
		{if (tmp[i].empty())tmp.erase(tmp.begin() + i--);}
	if (reason[0] == ':') reason.erase(reason.begin());
	else //shrink to the first space
		{for (size_t i = 0; i < reason.size(); i++){if (reason[i] == ' '){reason = reason.substr(0, i);break;}}}
	for (size_t i = 0; i < tmp.size(); i++){// erase the '#' from the channel name and check if the channel valid
			if (*(tmp[i].begin()) == '#')
				tmp[i].erase(tmp[i].begin());
			else
				{senderror(403, GetServerClient(fd)->GetNickName(), tmp[i], GetServerClient(fd)->GetFd(), " :No such channel\r\n"); tmp.erase(tmp.begin() + i--);}
		}
	return reason;
}

void	Server::KICK(std::string cmd, int fd)
{
	//ERR_BADCHANMASK (476) // if the channel mask is invalid
	std::vector<std::string> tmp;
	std::string reason ,user;
	reason = SplitCmdKick(cmd, tmp, user, fd);
	if (user.empty())
		{senderror(461, GetServerClient(fd)->GetNickName(), GetServerClient(fd)->GetFd(), " :Not enough parameters\r\n"); return;}
	for (size_t i = 0; i < tmp.size(); i++){ // search for the channel
		if (GetChannel(tmp[i])){// check if the channel exist
			Channel *ch = GetChannel(tmp[i]);
			if (!ch->GetChannelClient(fd) && !ch->get_admin(fd)) // check if the client is in the channel
				{senderror(442, GetServerClient(fd)->GetNickName(), "#" + tmp[i], GetServerClient(fd)->GetFd(), " :You're not on that channel\r\n"); continue;}
			if(ch->get_admin(fd)){ // check if the client is admin
				if (ch->GetServerClientInChannel(user)){ // check if the client to kick is in the channel
					std::stringstream ss;
					ss << ":" << GetServerClient(fd)->GetNickName() << "!~" << GetServerClient(fd)->GetUserName() << "@" << "localhost" << " KICK #" << tmp[i] << " " << user;
					if (!reason.empty())
						ss << " :" << reason << "\r\n";
					else ss << "\r\n";
					ch->sendToAll(ss.str());
					if (ch->get_admin(ch->GetServerClientInChannel(user)->GetFd()))
						ch->remove_admin(ch->GetServerClientInChannel(user)->GetFd());
					else
						ch->remove_client(ch->GetServerClientInChannel(user)->GetFd());
					if (ch->GetServerClientsNumber() == 0)
						channels.erase(channels.begin() + i);
				}
				else // if the client to kick is not in the channel
					{senderror(441, GetServerClient(fd)->GetNickName(), "#" + tmp[i], GetServerClient(fd)->GetFd(), " :They aren't on that channel\r\n"); continue;}
			}
			else // if the client is not admin
				{senderror(482, GetServerClient(fd)->GetNickName(), "#" + tmp[i], GetServerClient(fd)->GetFd(), " :You're not channel operator\r\n"); continue;}
		}
		else // if the channel doesn't exist
			senderror(403, GetServerClient(fd)->GetNickName(), "#" + tmp[i], GetServerClient(fd)->GetFd(), " :No such channel\r\n");
	}
}