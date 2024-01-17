CC = clang++
CFLAGS = -std=c++17 -Ofast -ffast-math -march=native -Wall -Wextra -fcolor-diagnostics -fansi-escape-codes -g -w -lCore -lRIO -lHist -lGraf -lGpad -lTree

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = .

ROOT_LIBS = `root-config --libs`
ROOT_CFLAGS = `root-config --cflags`
HDF5_LIBS = `pkg-config --libs hdf5` -lhdf5_cpp
HDF5_INCLUDE = `pkg-config --cflags hdf5` 
INCLUDES = -I$(SRC_DIR) $(ROOT_CFLAGS) $(HDF5_INCLUDE)

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

EXEC = $(BIN_DIR)/main

$(EXEC): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(OBJS) $(ROOT_LIBS) $(HDF5_LIBS) -o $(EXEC)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: clean
