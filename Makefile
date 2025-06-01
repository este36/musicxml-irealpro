.PHONY: all clean run tests

NAME = mxl2irp
CC = gcc

CFLAGS = -Wall -Wextra -g -fsanitize=address -std=c11 -I/usr/include/libxml2 
LFLAGS = -lxml2

OBJ_DIR = obj
SRC_DIR = src

SRC = main.c \
	  lib/sax.c

SRCS = $(addprefix $(SRC_DIR)/, $(SRC))
OBJS = $(addprefix $(OBJ_DIR)/, $(SRC:%.c=%.o))

all: $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LFLAGS)

clean:
	rm -rf $(OBJ_DIR)

run:
	./$(NAME)

tests:
	$(MAKE) -C tests
