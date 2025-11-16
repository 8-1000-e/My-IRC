/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edubois- <edubois-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 14:06:52 by edubois-          #+#    #+#             */
/*   Updated: 2025/11/16 15:46:52 by edubois-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

int main()
{
    Server serv;
    try
    {
        serv.ServerInit(10000, "oaui");
    }
    catch (std::exception e)
    {
        std::cerr << e.what();
    }
}