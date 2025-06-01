NAME = mxl2irp_tests 
CC = gcc

CFLAGS = -Wall -Wextra -g -fsanitize=address -std=c11 

all:
	$(CC) $(CFLAGS) all.c -o $(NAME)
	./$(NAME)

clean:
	rm $(NAME)

run:
	./$(NAME)
