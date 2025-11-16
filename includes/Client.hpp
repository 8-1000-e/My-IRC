/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edubois- <edubois-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 14:01:54 by edubois-          #+#    #+#             */
/*   Updated: 2025/11/16 18:24:07 by edubois-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>

class Client
{
    private:
        int Fd;
        std::string IPadd;
        
    public:
        Client();
        ~Client();
        
        int     GetFd;
        void    SetFd(int fd);
        void    SetIPadd(std::string IPadd);
};

#endif