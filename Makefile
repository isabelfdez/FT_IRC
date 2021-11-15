SRCS 		= main.cpp channel.cpp	./server/Server.cpp	user.cpp utils.cpp split.cpp ./server/messages.cpp \
				./command/userCmd.cpp ./command/nick.cpp ./command/join.cpp ./command/part.cpp  \
				./command/privmsg.cpp ./command/names.cpp ./command/notice.cpp ./command/pingPong.cpp ./command/quit.cpp\
				./command/mode.cpp ./command/invite.cpp ./command/kick.cpp ./command/oper.cpp ./command/topic.cpp \
				./command/ison.cpp ./command/list.cpp ./command/who.cpp
				

OBJS 		= $(SRCS:.cpp=.o)

CXX			= clang++ 

CXXFLAGS	= -Wall -Werror -Wextra -g3 -fsanitize=address

NAME		= ircserv

all:		$(NAME)

$(NAME):	$(OBJS)
			$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

clean:
			-@rm -f $(OBJS)

fclean: 	clean
			-@rm -f $(NAME)

re:			fclean all

.PHONY: all clea fclean re test