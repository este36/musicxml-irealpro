NAME = test
CC = gcc

CFLAGS = -Wall -Wextra -Werror -std=c11

INCLUDES_DIR = ./includes

OBJ_DIR = obj
SRC_DIR = src

SRC = main.c \
	  parser/musicxml_parser.c \
	  irealpro.c \
	  sax.c \
	  da.c \
	  gen/musicxml.c

SRCS = $(addprefix $(SRC_DIR)/, $(SRC))
OBJS = $(addprefix $(OBJ_DIR)/, $(SRC:%.c=%.o))

all: CFLAGS += -g
#all: CFLAGS += -O3
all: $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INCLUDES_DIR) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LFLAGS)

clean:
	rm -rf $(OBJ_DIR) $(NAME)

re: clean all

run: $(NAME)
	./$(NAME)

generate:
	python3 ./gen/musicxml.c.py > ./src/gen/musicxml.c
	python3 ./gen/musicxml.h.py > ./includes/musicxml.h
	python3 ./gen/musicxml_harmony.py > ./src/gen/musicxml_harmony.gperf
	gperf ./src/gen/musicxml_harmony.gperf > ./src/gen/musicxml_harmony.c

.PHONY: all re run clean
