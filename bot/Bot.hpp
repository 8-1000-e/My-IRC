/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edubois- <edubois-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 14:01:20 by edubois-          #+#    #+#             */
/*   Updated: 2025/11/25 13:26:56 by edubois-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
#define BOT_HPP


#include <iostream>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <map>
#include <string>
#include <algorithm>
#define auto __auto_type

#define RESET       "\033[0m"

#define BLACK       "\033[30m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define WHITE       "\033[37m"


    struct Info
    {
        int     nb;
        bool    marked;
        bool     validated;
        time_t  created_at;
        time_t  marked_at;
    };

#endif