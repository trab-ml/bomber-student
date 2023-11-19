# Compilateur
CC = gcc

# Options de compilation
CFLAGS = -Wall -Werror -g

# Répertoires
SRC_DIR = src
OBJ_DIR = bin
INC_DIR = include

# Fichiers sources
SRC = $(wildcard $(SRC_DIR)/*.c)

# Fichiers objet (dans le répertoire OBJ_DIR)
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

# Nom du fichier exécutable final (dans le répertoire OBJ_DIR)
TARGET = $(OBJ_DIR)/main

# Chemin des fichiers d'en-tête
INC = -I$(INC_DIR)

# Règles de compilation
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

# Règle de nettoyage
clean:
	rm -rf $(OBJ_DIR)/*
