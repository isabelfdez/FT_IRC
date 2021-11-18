SRCS 		= main.cpp	bot.cpp		split.cpp 	User.cpp
				
OBJS 		= $(SRCS:.cpp=.o)

CXX			= clang++ 

CXXFLAGS	= -Wall -Werror -Wextra -g3 -fsanitize=address

NAME		= bot

all:		$(NAME)

$(NAME):	$(OBJS)
			$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

clean:
			-@rm -f $(OBJS)

fclean: 	clean
			-@rm -f $(NAME)

re:			fclean all

.PHONY: all clea fclean re test