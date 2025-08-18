NAME = mxl2irp
LIB_NAME = lib$(NAME)

CC = gcc
INCLUDES_DIR = ./includes
CFLAGS = -Wall -Wextra -Werror -I$(INCLUDES_DIR)
MINIZ = test/vendors/libminiz

WASM_DIR = wasm
EMCC_LDFLAGS = \
	-sMODULARIZE=1\
	-sEXPORT_ES6=1\
	-sEXPORTED_FUNCTIONS='[\
		"_mxl_archive_create",\
		"_mxl_archive_free",\
		"_mxl_archive_append_file",\
		"_mxl_archive_get_musicxml_index",\
		"_mxl_archive_get_file_buf",\
		"_mxl_archive_get_file_len",\
		"_mxl_archive_get_files_count",\
		"_parse_musicxml",\
		"_irp_song_get_html",\
		"_irp_playlist_get_html",\
		"_irp_playlist_create",\
		"_irp_playlist_append",\
		"_irp_song_free",\
		"_irp_playlist_free",\
		"_free",\
		"_malloc"]'\
	-sEXPORTED_RUNTIME_METHODS=ccall,cwrap,HEAPU8

OBJ_DIR_STATIC = obj/static
OBJ_DIR_SHARED = obj/shared

SRC_DIR = src
BIN_DIR = build

LIB_SO = $(BIN_DIR)/$(LIB_NAME).so
LIB = $(BIN_DIR)/$(LIB_NAME).a
LIB_JS = $(BIN_DIR)/$(LIB_NAME).js

SRC = musicxml.c \
	parser/parse_measure.c \
	parser/parse_harmony.c \
	parser/parse_attributes.c \
	parser/parse_musicxml.c \
	mxl_archive.c \
	irealpro.c \
	url_encoder.c \
	irp_to_html.c \
	sax.c \
	da.c \

SRCS = $(addprefix $(SRC_DIR)/, $(SRC))
OBJS_STATIC = $(addprefix $(OBJ_DIR_STATIC)/, $(SRC:%.c=%.o))
OBJS_SHARED = $(addprefix $(OBJ_DIR_SHARED)/, $(SRC:%.c=%.o))
all: lib_a lib_js lib_so
lib_a: $(LIB)
lib_js: $(LIB_JS)
lib_so: $(LIB_SO)

do_tests: test/test.out
	python3 ./test/do_tests.py

test/test.out: $(LIB) $(LIB_JS)
	$(CC) $(CFLAGS) \
 		test/test.c \
		-I./$(MINIZ) -I./$(MINIZ) \
		-L./$(BIN_DIR) -L./$(MINIZ) \
		-l$(NAME) -lminiz \
		-o $@

$(LIB_JS): $(SRCS)
	mkdir -p $(BIN_DIR)
	docker run --rm -v $$(pwd):/src emscripten/emsdk bash -c "make wasm-emcc"

wasm-emcc: $(SRCS)
	emcc $(CFLAGS) $(SRCS) -o $(LIB_JS) $(EMCC_LDFLAGS)

$(LIB): CFLAGS += -g
$(LIB): $(OBJS_STATIC)
	mkdir -p $(BIN_DIR)
	ar -rc $@ $^
	

$(LIB_SO): CFLAGS += -fPIC
$(LIB_SO): $(OBJS_SHARED)
	mkdir -p $(BIN_DIR)
	$(CC) -shared $(CFLAGS) $^ -o $@ $(LFLAGS)

clean:
	rm -rf obj $(BIN_DIR)

$(OBJ_DIR_STATIC)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INCLUDES_DIR) -c $< -o $@

$(OBJ_DIR_SHARED)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INCLUDES_DIR) -c $< -o $@

serve: result.html
	nohup python3 -m http.server 8080 --bind 0.0.0.0 >/dev/null 2>&1 &

generate:
	python3 ./meta/musicxml.c.py > ./src/musicxml.c
	python3 ./meta/musicxml.h.py > ./includes/musicxml.h
	python3 ./meta/musicxml_harmony.py > ./src/musicxml_harmony.gperf
	gperf --language=ANSI-C ./src/musicxml_harmony.gperf > ./includes/irealpro_chord.h
	python3 meta/patch_gperf_header.py ./includes/irealpro_chord.h
	rm ./src/musicxml_harmony.gperf

.PHONY: do_tests all lib_js lib_so lib_a serve clean wasm-emcc
