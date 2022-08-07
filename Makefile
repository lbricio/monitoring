# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lbricio- <lbricio-@student.42sp.org.br>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/08/03 13:03:40 by lbricio-          #+#    #+#              #
#    Updated: 2022/08/03 13:44:40 by lbricio-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= monitoring

SRCS		=   src/monitoring.c\
				src/http_handler.c\
				src/ping_handler.c\
				src/utils.c
OBJS		= $(SRCS:.c=.o)
CXX			= cc
RM			= rm -f
CXXFLAGS	= -Wall -Wextra -Werror

all:		$(NAME)

$(NAME):	$(OBJS)
			$(CXX) $(CXXFLAGS) $(OBJS) -lcurl -o $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re:	fclean all

.PHONY:	all clean fclean re
