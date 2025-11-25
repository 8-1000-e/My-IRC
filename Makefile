# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: edubois- <edubois-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/02 11:09:48 by edubois-          #+#    #+#              #
#    Updated: 2025/11/17 13:05:02 by edubois-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

SRCS =  src/main.cpp\
		src/Server.cpp\
		src/Client.cpp\
		src/Auth.cpp\
		src/Channel.cpp\
		cmd/INVITE.cpp\
		cmd/JOIN.cpp\
		cmd/KICK.cpp\
		cmd/MODE.cpp\
		cmd/PART.cpp\
		cmd/PRIVMSG.cpp\
		cmd/QUIT.cpp\
		cmd/TOPIC.cpp\
		cmd/BOT.cpp\


OBJS = $(SRCS:.cpp=.o)

CC = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -I includes
RM = rm -f

all: $(NAME)
	@echo "Compilation terminée : $(NAME) est prêt !"

$(NAME): $(OBJS)
	$(CC) -o $(NAME) $(CXXFLAGS) $(OBJS)

%.o: %.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)
	@echo "Les fichiers objets ont été supprimés."

fclean: clean
	$(RM) $(NAME)
	@echo "L'exécutable $(NAME) a été supprimé."

re: fclean all

.PHONY: all clean fclean re