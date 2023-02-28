SRC_DIR	:= src
OBJ_DIR	:= obj
BIN_DIR	:= bin

EXE	:= $(BIN_DIR)/KAT1
SRC	:= $(wildcard $(SRC_DIR)/*.cpp)
OBJ	:= $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

CPPFLAGS	:= -Iinclude -MMD -MP
CXXFLAGS	:= -Wall -std=c++17
LDFLAGS	:= -Llib
LDLIBS	:= -lm

.PHONY:	all clean

all:	$(EXE)

$(EXE):	$(OBJ) | $(BIN_DIR)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o:	$(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

run:	$(EXE)
	$(EXE) $(ARGS)

clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR)

-include $(OBJ:.o=.d)