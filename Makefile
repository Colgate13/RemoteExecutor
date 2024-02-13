SRC_DIR = src
BIN_DIR = bin

CC = clang # compiler
LIB_INCLUDE = -I/usr/local/include # path to headers
LIB_PATH = -L/usr/local/lib # path to libraries
CFLAGS =-g -Wall -Wextra $(LIB_INCLUDE) # compiler flags
TARGET = main # output file
SOURCE_FILE = $(SRC_DIR)/main.c $(SRC_DIR)/keyLogger.c # source file
SOURCE_FILE_PERSISTENCE = $(SRC_DIR)/persistence.c # source file

LIBS_ALL = $(LIB_PATH)
OUTPUT_FILE = $(BIN_DIR)/$(TARGET)
OUTPUT_FILE_PERSISTENCE = $(BIN_DIR)/persistence

$(info Compile Project with: $(CC))
$(info OUTPUT_FILE: $(OUTPUT_FILE))

all: clean $(OUTPUT_FILE)

$(OUTPUT_FILE): $(SOURCE_FILE) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(OUTPUT_FILE) $(SOURCE_FILE) $(LIBS_ALL)

persistence: $(SOURCE_FILE) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(OUTPUT_FILE_PERSISTENCE) $(SOURCE_FILE_PERSISTENCE) $(LIBS_ALL) -D PERSISTENCE

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(BIN_DIR)/$(TARGET)
