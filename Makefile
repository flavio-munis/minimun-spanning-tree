# MakeFile For Minimum Spanning Tree

# Project Name and Number Generator
PROJECT_NAME = minimum-spanning-tree

# Compilation Options
CC = gcc
CC_FLAGS = -g \
	-Wall \
	-pedantic \
	-o
MAIN_OBJ = minimum-spanning-tree.c

# Compile all files and creates the necessary folders for the project

all: $(PROJECT_NAME)

$(PROJECT_NAME): $(MAIN_OBJ)
	@ echo 'Compiling Executable File...'
	@ $(CC) $^ $(CC_FLAGS) $@
	@ echo 'Finished Building Project!'


.PHONY: clean
clean:
	@ echo "Cleaning All Core Files..."
	@ rm -f ./core*
