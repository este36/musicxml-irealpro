NAME = mxl2irp
LIB_NAME = lib$(NAME)

CC = gcc
INCLUDES_DIR = ./includes
CFLAGS = -Wall -Wextra -Werror
MINIZ = test/vendors/libminiz

WASM_DIR = wasm
EMCC_LDFLAGS = \
	-sMODULARIZE=1\
	-sEXPORT_ES6=1\
	-sEXPORTED_FUNCTIONS='["_free", "_malloc"]'\
	-sEXPORTED_RUNTIME_METHODS=ccall,cwrap,HEAPU8

OBJ_DIR_STATIC = obj/static
OBJ_DIR_SHARED = obj/shared

SRC_DIR = src
BIN_DIR = build

LIB_SO = $(BIN_DIR)/$(LIB_NAME).so
LIB = $(BIN_DIR)/$(LIB_NAME).a
LIB_JS = $(BIN_DIR)/$(LIB_NAME).js

SRC = mxl2irp.c \
	musicxml.c \
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
	python3 ./test/jazz1460.py

test/test.out: $(LIB) $(LIB_JS)
	$(CC) $(CFLAGS) test/test.c -I$(INCLUDES_DIR) -I./$(MINIZ) ./$(LIB) ./$(MINIZ)/libminiz.a -o $@

$(LIB_JS):
	mkdir -p $(BIN_DIR)
	docker run --rm -v $$(pwd):/src emscripten/emsdk bash -c "make wasm-emcc"

wasm-emcc:
	emcc $(CFLAGS) -Oz $(SRCS) -I$(INCLUDES_DIR) -o $(LIB_JS) $(EMCC_LDFLAGS)

$(LIB): CFLAGS += -g
$(LIB): $(OBJS_STATIC)
	mkdir -p $(BIN_DIR)
	ar -rc $@ $^
	

$(LIB_SO): CFLAGS += -fPIC
$(LIB_SO): $(OBJS_SHARED)
	mkdir -p $(BIN_DIR)
	$(CC) -shared $(CFLAGS) $^ -o $@ $(LFLAGS)

clean:
	rm -rf obj $(BIN_DIR) ./test/test.out

re: clean all

$(OBJ_DIR_STATIC)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INCLUDES_DIR) -c $< -o $@

$(OBJ_DIR_SHARED)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INCLUDES_DIR) -c $< -o $@

serve:
	python3 -m http.server 8080 --bind 0.0.0.0 >/dev/null 2>&1 &

generate:
	python3 ./meta/musicxml.c.py > ./src/musicxml.c
	python3 ./meta/musicxml.h.py > ./includes/musicxml.h
	python3 ./meta/musicxml_harmony.py > ./src/musicxml_harmony.gperf
	gperf --language=ANSI-C ./src/musicxml_harmony.gperf > ./includes/irealpro_chord.h
	python3 meta/patch_gperf_header.py ./includes/irealpro_chord.h
	rm ./src/musicxml_harmony.gperf

.PHONY: do_tests all re lib_js lib_so lib_a serve clean wasm-emcc
