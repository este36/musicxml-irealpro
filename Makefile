NAME = mxl2irp
LIB_NAME = lib$(NAME)

CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
INCLUDES_DIR = ./includes

WASM_DIR = wasm
EMCC_LDFLAGS = \
	-sEXPORTED_FUNCTIONS='["_parse_musicxml_song",\
		"_irp_get_song_html",\
		"_irp_get_playlist_html",\
		"_irp_song_free",\
		"_free",\
		"_malloc"]'\
	-sEXPORTED_RUNTIME_METHODS=ccall,cwrap,UTF8ToString,allocateUTF8,HEAPU8

OBJ_DIR = obj
SRC_DIR = src
BIN_DIR = bin

LIB_SO = $(BIN_DIR)/$(LIB_NAME).so

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

test: $(LIB_SO) wasm
	$(CC) $(CFLAGS) -I$(INCLUDES_DIR) test/test.c -L$$(pwd)/$(BIN_DIR) -l$(NAME) -Wl,-rpath=$$(pwd)/$(BIN_DIR) -o $(BIN_DIR)/test.out

wasm:
	mkdir -p $(BIN_DIR)
	docker run --rm -v $$(pwd):/src emscripten/emsdk bash -c "make wasm-emcc"

wasm-emcc:
	emcc $(SRCS) -o $(BIN_DIR)/$(LIB_NAME).js -I$(INCLUDES_DIR) $(EMCC_LDFLAGS)

$(LIB_SO): CFLAGS += -g
$(LIB_SO): CFLAGS += -fPIC
$(LIB_SO): $(OBJS)
	mkdir -p $(BIN_DIR)
	$(CC) -shared $(CFLAGS) $(OBJS) -o $(LIB_SO) $(LFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INCLUDES_DIR) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

re: clean test

serve: result.html
	nohup python3 -m http.server 8080 --bind 0.0.0.0 >/dev/null 2>&1 &

generate:
	python3 ./gen/musicxml.c.py > ./src/musicxml.c
	python3 ./gen/musicxml.h.py > ./includes/musicxml.h
	python3 ./gen/musicxml_harmony.py > ./src/musicxml_harmony.gperf
	gperf --language=ANSI-C ./src/musicxml_harmony.gperf > ./includes/irealpro_chord.h
	python3 gen/patch_gperf_header.py ./includes/irealpro_chord.h
	rm ./src/musicxml_harmony.gperf

.PHONY: all re run clean wasm wasm-emcc
