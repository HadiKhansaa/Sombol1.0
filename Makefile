GCC = g++
GCC_FLAGS = -Ofast -faggressive-loop-optimizations
OBJ_DIR = bin
INCL_DIR = header
SRC_DIR = src
OBJ = $(OBJ_DIR)/AI.o $(OBJ_DIR)/checking.o $(OBJ_DIR)/hashing.o $(OBJ_DIR)/moveGeneration.o $(OBJ_DIR)/util.o $(OBJ_DIR)/main.o $(OBJ_DIR)/globals.o 
EXE = sombol1.exe

default: $(EXE)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(GCC) -I $(INCL_DIR) $(GCC_FLAGS) -c -o $@ $<

$(EXE): $(OBJ)
	$(GCC) -I $(INCL_DIR) $(GCC_FLAGS) $(OBJ) -o $(EXE)

clean:
	del /f /q $(OBJ_DIR)\*.o $(EXE)
