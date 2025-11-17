/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TOPIC.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edubois- <edubois-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 13:04:28 by edubois-          #+#    #+#             */
/*   Updated: 2025/11/17 13:04:33 by edubois-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

std::string Server::tTopic()
{
	std::time_t current = std::time(NULL);
	std::stringstream res;

	res << current;
	return res.str();
}
std::string Server::gettopic(std::string &input)
{
	size_t pos = input.find(":");
	if (pos == std::string::npos)
	{
		return "";
	}
	return input.substr(pos);
}

int Server::getpos(std::string &cmd)
{
	for (int i = 0; i < (int)cmd.size(); i++)
		if (cmd[i] == ':' && (cmd[i - 1] == 32))
			return i;
	return -1;
}

void Server::Topic(std::string &cmd, int &fd)
{
	if (cmd == "TOPIC :")
		{senderror(461, GetServerClient(fd)->GetNickName(), fd, " :Not enough parameters\r\n");return;} // ERR_NEEDMOREPARAMS (461) if there are not enough parameters
	std::vector<std::string> scmd = splitCmd(cmd);
	if (scmd.size() == 1)
		{senderror(461, GetServerClient(fd)->GetNickName(), fd, " :Not enough parameters\r\n");return;} // ERR_NEEDMOREPARAMS (461) if there are not enough parameters
	std::string nmch = scmd[1].substr(1);
	if (!GetChannel(nmch)) // ERR_NOSUCHCHANNEL (403) if the given channel does not exist
		{senderror(403, "#" + nmch, fd, " :No such channel\r\n"); return;}
	if (!(GetChannel(nmch)->GetChannelClient(fd)) && !(GetChannel(nmch)->get_admin(fd)))
		{senderror(442, "#" + nmch, fd, " :You're not on that channel\r\n");return;} // ERR_NOTONCHANNEL (442) if the client is not on the channel
	if (scmd.size() == 2)
	{
		if (GetChannel(nmch)->GetTopicName() == "")
		{_sendResponse(": 331 " + GetServerClient(fd)->GetNickName() + " " + "#" + nmch + " :No topic is set\r\n", fd);return;} // RPL_NOTOPIC (331) if no topic is set
		size_t pos = GetChannel(nmch)->GetTopicName().find(":");
		if (GetChannel(nmch)->GetTopicName() != "" && pos == std::string::npos)
		{
			_sendResponse(": 332 " + GetServerClient(fd)->GetNickName() + " " + "#" + nmch + " " + GetChannel(nmch)->GetTopicName() + "\r\n", fd);			  // RPL_TOPIC (332) if the topic is set
			_sendResponse(": 333 " + GetServerClient(fd)->GetNickName() + " " + "#" + nmch + " " + GetServerClient(fd)->GetNickName() + " " + GetChannel(nmch)->GetTime() + "\r\n", fd); // RPL_TOPICWHOTIME (333) if the topic is set
			return;
		}
		else
		{
			size_t pos = GetChannel(nmch)->GetTopicName().find(" ");
			if (pos == 0)
				GetChannel(nmch)->GetTopicName().erase(0, 1);
			_sendResponse(": 332 " + GetServerClient(fd)->GetNickName() + " " + "#" + nmch + " " + GetChannel(nmch)->GetTopicName() + "\r\n", fd);			  // RPL_TOPIC (332) if the topic is set
			_sendResponse(": 333 " + GetServerClient(fd)->GetNickName() + " " + "#" + nmch + " " + GetServerClient(fd)->GetNickName() + " " + GetChannel(nmch)->GetTime() + "\r\n", fd); // RPL_TOPICWHOTIME (333) if the topic is set
			return;
		}
	}

	if (scmd.size() >= 3)
	{
		std::vector<std::string> tmp;
		int pos = getpos(cmd);
		if (pos == -1 || scmd[2][0] != ':')
		{
			tmp.push_back(scmd[0]);
			tmp.push_back(scmd[1]);
			tmp.push_back(scmd[2]);
		}
		else
		{
			tmp.push_back(scmd[0]);
			tmp.push_back(scmd[1]);
			tmp.push_back(cmd.substr(getpos(cmd)));
		}

		if (tmp[2][0] == ':' && tmp[2][1] == '\0')
		{senderror(331, "#" + nmch, fd, " :No topic is set\r\n");return;} // RPL_NOTOPIC (331) if no topic is set

		if (GetChannel(nmch)->GettopicRestriction() && GetChannel(nmch)->GetChannelClient(fd))
		{senderror(482, "#" + nmch, fd, " :You're Not a channel operator\r\n");return;} // ERR_CHANOPRIVSNEEDED (482) if the client is not a channel operator
		else if (GetChannel(nmch)->GettopicRestriction() && GetChannel(nmch)->get_admin(fd))
		{
			GetChannel(nmch)->SetTime(tTopic());
			GetChannel(nmch)->SetTopicName(tmp[2]);
			std::string rpl;
			size_t pos = tmp[2].find(":");
			if (pos == std::string::npos)
				rpl = ":" + GetServerClient(fd)->GetNickName() + "!" + GetServerClient(fd)->GetUserName() + "@localhost TOPIC #" + nmch + " :" + GetChannel(nmch)->GetTopicName() + "\r\n"; // RPL_TOPIC (332) if the topic is set
			else
				rpl = ":" + GetServerClient(fd)->GetNickName() + "!" + GetServerClient(fd)->GetUserName() + "@localhost TOPIC #" + nmch + " " + GetChannel(nmch)->GetTopicName() + "\r\n"; // RPL_TOPIC (332) if the topic is set
			GetChannel(nmch)->sendToAll(rpl);
		}
		else
		{
			std::string rpl;
			size_t pos = tmp[2].find(":");
			if (pos == std::string::npos)
			{
				GetChannel(nmch)->SetTime(tTopic());
				GetChannel(nmch)->SetTopicName(tmp[2]);
				rpl = ":" + GetServerClient(fd)->GetNickName() + "!" + GetServerClient(fd)->GetUserName() + "@localhost TOPIC #" + nmch + " " + GetChannel(nmch)->GetTopicName() + "\r\n"; // RPL_TOPIC (332) if the topic is set
			}
			else
			{
				size_t poss = tmp[2].find(" ");
				GetChannel(nmch)->SetTopicName(tmp[2]);
				if (poss == std::string::npos && tmp[2][0] == ':' && tmp[2][1] != ':')
					tmp[2] = tmp[2].substr(1);
				GetChannel(nmch)->SetTopicName(tmp[2]);
				GetChannel(nmch)->SetTime(tTopic());
				rpl = ":" + GetServerClient(fd)->GetNickName() + "!" + GetServerClient(fd)->GetUserName() + "@localhost TOPIC #" + nmch + " " + GetChannel(nmch)->GetTopicName() + "\r\n"; // RPL_TOPIC (332) if the topic is set
			}
			GetChannel(nmch)->sendToAll(rpl);
		}
	}
}