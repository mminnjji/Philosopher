CC = cc
CFLAGS = -Wall -Wextra -Werror
NAME	= philo

SRCS 	= main.c util.c work.c ft_atoi.c
OBJS 	= $(SRCS:.c=.o)

.c.o :	$(SRCS)
	$(CC) -c $< -o $(<:.c=.o)

all : $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME) -g -fsanitize=address

clean:
	rm -f $(OBJS)

fclean: 	clean
	rm -f $(NAME)

re:	fclean all

.PHONY: all clean fclean re