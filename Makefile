NAME = mxl2irp
CC = gcc

CFLAGS = -Wall -Wextra -std=c11 -I/usr/include/libxml2
LFLAGS = -lxml2

OBJ_DIR = obj
SRC_DIR = src

SRC = main.c \
      mongoose.c \
	  mxl2irp.c \

SRCS = $(addprefix $(SRC_DIR)/, $(SRC))
OBJS = $(addprefix $(OBJ_DIR)/, $(SRC:%.c=%.o))

all: $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LFLAGS)

clean:
	rm -rf $(OBJ_DIR)

run:
	./$(NAME)

css:
	tailwindcss -i assets/main.css -o public/style.css --minify
