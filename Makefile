GCC = g++
GCC_FLAGS = -Ofast -faggressive-loop-optimizations
DEBUG_FLAGS = -g -Og
OBJ_DIR = bin
INCL_DIR = header
SRC_DIR = src
OBJ = $(OBJ_DIR)/AI.o $(OBJ_DIR)/checking.o $(OBJ_DIR)/hashing.o $(OBJ_DIR)/moveGeneration.o $(OBJ_DIR)/util.o $(OBJ_DIR)/main.o $(OBJ_DIR)/globals.o 
EXE = sombol1.exe
DEBUG_EXE = sombol1_debug.exe

default: $(EXE)

debug: GCC_FLAGS = $(DEBUG_FLAGS)  # Override GCC_FLAGS with DEBUG_FLAGS
debug: EXE = $(DEBUG_EXE)  # Change executable name for debug mode
debug: $(EXE)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(GCC) -I $(INCL_DIR) $(GCC_FLAGS) -c -o $@ $<

$(EXE): $(OBJ)
	$(GCC) -I $(INCL_DIR) $(GCC_FLAGS) $(OBJ) -o $(EXE)

clean:
	del /f /q $(OBJ_DIR)\*.o $(EXE) $(DEBUG_EXE)
