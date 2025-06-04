.PHONY: all clean run tests debug

NAME = mxl2irp
CC = gcc

CFLAGS = -Wall -Wextra -std=c11

OBJ_DIR = obj
SRC_DIR = src

SRC = main.c \
	  lib/irealpro.c \
	  lib/parser.c \
	  lib/sax.c \
	  lib/da.c \
	  lib/musicxml.c

SRCS = $(addprefix $(SRC_DIR)/, $(SRC))
OBJS = $(addprefix $(OBJ_DIR)/, $(SRC:%.c=%.o))

all: CFLAGS += -fsanitize=address
all: $(NAME)

debug: CFLAGS += -DDEBUG -g -O0 
debug: $(NAME)


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LFLAGS)

clean:
	rm -rf $(OBJ_DIR) $(NAME)

run: $(NAME)
	./$(NAME)

tests:
	$(MAKE) -C tests

