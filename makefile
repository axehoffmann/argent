CMP = g++
CMP_FLAGS = -g -Wall
SRC = src
OBJ = obj

SUBDIRS = (/src/*/)

BIN_DIR = bin
BIN = $(BIN_DIR)/main

INC := $(shell tr '\n' ' -I/' < includes.txt)

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRCS = $(call rwildcard, $(SRC), *.cpp)

OBJS = $(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SRCS))

TEST = tests
TESTS = $(wildcard $(TEST)/**.cpp)
TEST_BINS = $(patsubst $(TEST)/%.cpp, $(TEST)/bin/%, $(TESTS))

.SILENT:

all: $(BIN)

release: CMP_FLAGS = -Wall -O2 -DNDEBUG
release: clean
release: $(BIN)


$(BIN): $(OBJS)
	mkdir -p $@
	$(CMP) $(CMP_FLAGS) $(OBJS) -o $@

$(OBJ)/%.o: $(SRC)/%.cpp
	$(CMP) $(CMP_FLAGS) -I$(INC) -c $< -o $@


$(TEST)/bin/%: $(TEST)/%.cpp
	$(CMP) $(CMP_FLAGS) -o $@ -I$(INC) $< $(OBJS) -lcriterion 

$(TEST)/bin:
	mkdir -p $@
$(OBJ):
	mkdir -p $@

test: clean
test: $(OBJS) $(TEST)/bin $(TEST_BINS)
	for test in $(TEST_BINS) ; do ./$$test ; done

clean:
	$(RM) $(BIN_DIR)/main/*
	find $(OBJ) -name '*.o' -delete