# compilator: gcc
CC = gcc

# compilation flags
CFLAGS = -Wall -Werror -g

# directories
SRC_DIR = src
OBJ_DIR = bin
INC_DIR = include

# associated files
SRC = $(wildcard $(SRC_DIR)/*.c)

# object files in OBJ_DIR directory
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

# executable filename in OBJ_DIR directory
TARGET = $(OBJ_DIR)/main

# include path
INC = -I$(INC_DIR)

# default target
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	rm -rf $(OBJ_DIR)/*

debug:
	@echo "Source files: $(SRC)"
	@echo "Object files: $(OBJ)"
	@echo "Target: $(TARGET)"
	@echo "Include path: $(INC)"