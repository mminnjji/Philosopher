CC = cc
CFLAGS = -Wall -Wextra -Werror
NAME	= philo

SRCS 	= srcs/main.c srcs/util.c srcs/work.c srcs/ft_atoi.c srcs/check.c srcs/thread.c
OBJS 	= $(SRCS:.c=.o)

.c.o :	$(SRCS)
	$(CC) -c $< -o $(<:.c=.o)

all : $(NAME)

$(NAME): $(OBJS)
	$(CC) -g $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: 	clean
	rm -f $(NAME)

re:	fclean all

.PHONY: all clean fclean re