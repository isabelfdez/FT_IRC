SRCS 		= main.cpp	channel.cpp	server.cpp	user.cpp	utils.cpp	commands.cpp

OBJS 		= $(SRCS:.cpp=.o)

CXX			= clang++ 

CXXFLAGS	= -Wall -Werror -Wextra -std=c++98

NAME		= IRC

all:		$(NAME)

$(NAME):	$(OBJS)
			$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

clean:
			-@rm -f $(OBJS)

fclean: 	clean
			-@rm -f $(NAME)

re:			fclean all

.PHONY: all clea fclean re test