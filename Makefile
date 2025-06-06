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
	  gen/musicxml.c

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

generate:
	python3 ./gen/musicxml.c.py > ./src/gen/musicxml.c
	python3 ./gen/musicxml.h.py > ./src/gen/musicxml.h
	python3 ./gen/musicxml_harmony.py > ./src/gen/musicxml_harmony.gperf
	gperf ./src/gen/musicxml_harmony.gperf > ./src/gen/musicxml_harmony.c
