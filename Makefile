NAME=ircserv

SRCS = main.cpp

OBJS = $(SRCS:.cpp=.o)

CC = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++98

RM = rm -fr

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CXXFLAGS) -o $(NAME) $(OBJS)

%.o: %.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re