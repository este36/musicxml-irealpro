NAME = mxl2irp
LIB_NAME = lib$(NAME)

CC = gcc
CFLAGS = -Wall -Wextra -Werror
MINIZ_DIR = vendors/libminiz
INCLUDES = -I./includes -I./$(MINIZ_DIR)

WASM_DIR = wasm
EMCC_LDFLAGS = \
	-sINITIAL_MEMORY=1MB\
	-sALLOW_MEMORY_GROWTH\
	-sMAXIMUM_MEMORY=32MB\
	-sMODULARIZE=1\
	-sEXPORT_ES6=1\
	-sEXPORTED_FUNCTIONS='["_free", "_malloc"]'\
	-sEXPORTED_RUNTIME_METHODS=ccall,cwrap,HEAPU8,UTF8ToString

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

MINIZ_FILES = miniz.c \
	miniz_zip.c \
	miniz_tinfl.c \
	miniz_tdef.c \

SRCS = $(addprefix $(SRC_DIR)/, $(SRC)) \
	$(addprefix $(MINIZ_DIR)/, $(MINIZ_FILES))
OBJS_STATIC = $(addprefix $(OBJ_DIR_STATIC)/, $(SRC:%.c=%.o)) \
	$(addprefix $(OBJ_DIR_STATIC)/$(MINIZ_DIR)/, $(MINIZ_FILES:%.c=%.o))
OBJS_SHARED = $(addprefix $(OBJ_DIR_SHARED)/, $(SRC:%.c=%.o)) \
	$(addprefix $(OBJ_DIR_SHARED)/$(MINIZ_DIR)/, $(MINIZ_FILES:%.c=%.o))

all: lib_a lib_js lib_so $(NAME)
lib_a: $(LIB)
lib_js: $(LIB_JS)
lib_so: $(LIB_SO)

$(NAME): $(LIB) 
	$(CC) $(CFLAGS) cli/main.c $(INCLUDES) ./$(LIB) -o $@

$(LIB_JS):
	mkdir -p $(BIN_DIR)
	docker run --rm -v $$(pwd):/src emscripten/emsdk bash -c "make wasm-emcc"

wasm-emcc:
	emcc $(CFLAGS) -Oz $(SRCS) $(INCLUDES) -o $(LIB_JS) $(EMCC_LDFLAGS)

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

re: clean $(NAME)

OPTI = -O2

$(OBJ_DIR_STATIC)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(OPTI) $(INCLUDES) -c $< -o $@

$(OBJ_DIR_SHARED)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(OPTI) $(INCLUDES) -c $< -o $@

$(OBJ_DIR_STATIC)/$(MINIZ_DIR)/%.o: $(MINIZ_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(OPTI) $(INCLUDES) -c $< -o $@

$(OBJ_DIR_SHARED)/$(MINIZ_DIR)/%.o: $(MINIZ_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(OPTI) $(INCLUDES) -c $< -o $@

generate:
	python3 ./meta/musicxml.c.py > ./src/musicxml.c
	python3 ./meta/musicxml.h.py > ./includes/musicxml.h
	python3 ./meta/musicxml_harmony.py > ./src/musicxml_harmony.gperf
	gperf --language=ANSI-C ./src/musicxml_harmony.gperf > ./includes/irealpro_chord.h
	python3 meta/patch_gperf_header.py ./includes/irealpro_chord.h
	rm ./src/musicxml_harmony.gperf

init_tests:
	git clone https://github.com/este36/musicxml-irealpro-test-files.git test

do_tests:
	python3 test/do_tests.py

.PHONY: all re lib_js lib_so lib_a serve clean wasm-emcc
