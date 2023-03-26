CMP = g++
CMP_FLAGS = -g -Wall
SRC = src
OBJ = obj

BIN_DIR = bin
BIN = $(BIN_DIR)/main

INC := $(shell tr '\n' ' ' < includes.txt)

ECS = $(SRC)/ecs
ECS_SRCS = $(wildcard $(ECS)/**.cpp)

OBJS = $(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(ECS_SRCS))

TEST = tests
TESTS = $(wildcard $(TEST)/**.cpp)
TEST_BINS = $(patsubst $(TEST)/%.cpp, $(TEST)/bin/%, $(TESTS))

.SILENT:

all: $(BIN)

release: CMP_FLAGS = -Wall -O2 -DNDEBUG
release: clean
release: $(BIN)


$(BIN): $(OBJS)
	$(CMP) $(CMP_FLAGS) $(OBJS) -o $@

$(OBJ)/%.o: $(SRC)/%.cpp
	$(CMP) $(CMP_FLAGS) $(INC) -c $< -o $@


$(TEST)/bin/%: $(TEST)/%.cpp
	$(CMP) $(CMP_FLAGS) $< $(OBJS) -o $@ -lcriterion

$(BIN):
	mkdir $@
$(TEST)/bin:
	mkdir $@
$(OBJ):
	mkdir $@

test: $(TEST)/bin $(TEST_BINS)
	for test in $(TEST_BINS) ; do ./$$test ; done

clean:
	$(RM) -r $(OBJ)/* $(BIN_DIR)/*