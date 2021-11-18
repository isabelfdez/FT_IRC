SRCS 		= main.cpp channel.cpp	./server/Server.cpp	user.cpp utils.cpp split.cpp ./server/messages.cpp \
				./command/userCmd.cpp ./command/nick.cpp ./command/join.cpp ./command/part.cpp  \
				./command/privmsg.cpp ./command/names.cpp ./command/notice.cpp ./command/pingPong.cpp ./command/quit.cpp\
				./command/mode.cpp ./command/invite.cpp ./command/kick.cpp ./command/oper.cpp ./command/topic.cpp \
				./command/ison.cpp ./command/list.cpp ./command/who.cpp	./command/pass.cpp
				

OBJS 		= $(SRCS:.cpp=.o)

CXX			= clang++ 

CXXFLAGS	= -Wall -Werror -Wextra -g3 -fsanitize=address

NAME		= ircserv

BOT			= bot

all:		$(NAME)

$(NAME):	$(OBJS)
			$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

clean:
			-@rm -f $(OBJS)
			-@$(MAKE) -C bot clean


fclean: 	clean
			-@rm -f $(NAME)
			-@$(MAKE) -C bot fclean


re:			fclean all

bonus:		$(OBJS)
			$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)
			$(MAKE) -C bot

.PHONY: all clea fclean re test