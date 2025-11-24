/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edubois- <edubois-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 14:01:20 by edubois-          #+#    #+#             */
/*   Updated: 2025/11/24 14:26:42 by edubois-         ###   ########.fr       */
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

    struct Info
    {
        int     nb;
        bool    marked;
        time_t  created_at;
        time_t  marked_at;
    };

#endif