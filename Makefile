LIB_NAME = mxl_to_irealpro
NAME = lib$(LIB_NAME).so
CC = gcc

CFLAGS = -Wall -Wextra -Werror -std=c11

INCLUDES_DIR = ./includes

EMCC_LDFLAGS = \
		-sEXPORTED_FUNCTIONS=_parse_musicxml_song,_irp_get_song_html,_irp_get_playlist_html,_irp_song_free,_free,_malloc \
		-sEXPORTED_RUNTIME_METHODS=ccall,cwrap,UTF8ToString,allocateUTF8,HEAPU8

OBJ_DIR = obj
SRC_DIR = src

SRC = musicxml.c \
	parser/parse_measure.c \
	parser/parse_harmony.c \
	parser/parse_attributes.c \
	parser/parse_musicxml_song.c \
	irealpro.c \
	url_encoder.c \
	irp_to_html.c \
	sax.c \
	da.c \

SRCS = $(addprefix $(SRC_DIR)/, $(SRC))
OBJS = $(addprefix $(OBJ_DIR)/, $(SRC:%.c=%.o))

all: CFLAGS += -fPIC
#all: CFLAGS += -O3
all: $(NAME)

test: re
	$(CC) $(CFLAGS) -I$(INCLUDES_DIR) test.c -L. -l$(LIB_NAME) -Wl,-rpath=. -o test

wasm:
	mkdir -p build-wasm
	emcc $(SRCS) -o build-wasm/$(LIB_NAME).js -I$(INCLUDES_DIR) $(EMCC_LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INCLUDES_DIR) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) -shared $(CFLAGS) $(OBJS) -o $(NAME) $(LFLAGS)

clean:
	rm -rf $(OBJ_DIR) $(NAME) test

re: clean all

run: $(NAME)
	./$(NAME)

generate:
	python3 ./gen/musicxml.c.py > ./src/musicxml.c
	python3 ./gen/musicxml.h.py > ./includes/musicxml.h
	python3 ./gen/musicxml_harmony.py > ./src/musicxml_harmony.gperf
	gperf --language=ANSI-C ./src/musicxml_harmony.gperf > ./includes/irealpro_chord.h
	python3 gen/patch_gperf_header.py ./includes/irealpro_chord.h
	rm ./src/musicxml_harmony.gperf

.PHONY: all re run clean
