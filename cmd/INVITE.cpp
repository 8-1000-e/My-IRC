/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   INVITE.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edubois- <edubois-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 13:04:08 by edubois-          #+#    #+#             */
/*   Updated: 2025/11/17 13:04:10 by edubois-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

void Server::Invite(std::string &cmd, int &fd)
{
	std::vector<std::string> scmd = splitCmd(cmd);
	if(scmd.size() < 3)// ERR_NEEDMOREPARAMS (461) if there are not enough parameters
		{senderror(461, GetServerClient(fd)->GetNickName(), fd, " :Not enough parameters\r\n"); return;}
	std::string channelname = scmd[2].substr(1);
	if(scmd[2][0] != '#' || !GetChannel(channelname))// ERR_NOSUCHCHANNEL (403) if the given channel does not exist
	    {senderror(403, channelname, fd, " :No such channel\r\n"); return;}
	if (!(GetChannel(channelname)->GetChannelClient(fd)) && !(GetChannel(channelname)->get_admin(fd)))// ERR_NOTONCHANNEL (442) if the client is not on the channel
	    {senderror(442, channelname, fd, " :You're not on that channel\r\n"); return;}
	if (GetChannel(channelname)->GetServerClientInChannel(scmd[1]))// ERR_USERONCHANNEL (443) if the given nickname is already on the channel
	    {senderror(443, GetServerClient(fd)->GetNickName(), channelname, fd, " :is already on channel\r\n"); return;}
	Client *clt = GetServerClientNick(scmd[1]);
	if (!clt)// ERR_NOSUCHNICK (401) if the given nickname is not found
		{senderror(401, scmd[1], fd, " :No such nick\r\n");return;}
	if (GetChannel(channelname)->GetInvitOnly() && !GetChannel(channelname)->get_admin(fd))// ERR_INVITEONLYCHAN (473) if the channel is invite-only
		{senderror(482,GetChannel(channelname)->GetChannelClient(fd)->GetNickName(),scmd[1],fd," :You're not channel operator\r\n"); return;}
	if (GetChannel(channelname)->GetLimit() && GetChannel(channelname)->GetServerClientsNumber() >= GetChannel(channelname)->GetLimit()) // ERR_CHANNELISFULL (471) if the channel is full
		{senderror(473,GetChannel(channelname)->GetChannelClient(fd)->GetNickName(),channelname,fd," :Cannot invit to channel (+i)\r\n"); return;}
	// RPL_INVITING (341) if the invite was successfully sent
	clt->AddChannelInvite(channelname);
	std::string rep1 = ": 341 "+ GetServerClient(fd)->GetNickName()+" "+ clt->GetNickName()+" "+ scmd[2]+"\r\n";
	_sendResponse(rep1, fd);
	std::string rep2 = ":"+ clt->getHostname() + " INVITE " + clt->GetNickName() + " " + scmd[2]+"\r\n";
	_sendResponse(rep2, clt->GetFd());
	
}