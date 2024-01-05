LIB-NAME-DEPENDICIES := -lyaml-cpp -lSDL2
HEADER-DEPENDICIES := /usr/include/eigen3 

CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -D_REENTRANT -I $(HEADER-DEPENDICIES) $(LIB-NAME-DEPENDICIES)

SRC-PARENT-DIR := src
SRC-DIRS := emu config

OBJ-DIR := obj
BIN-DIR := bin

# Collect all source files
SOURCES := $(foreach srcDir, $(SRC-DIRS), $(wildcard $(addprefix $(SRC-PARENT-DIR)/, $(srcDir))/*.cpp))

# Generate a list of object files from source files
OBJECTS := $(patsubst $(SRC-PARENT-DIR)/%.cpp, $(OBJ-DIR)/%.o, $(SOURCES))

# Generate a list of executable targets
PROGRAMS := $(addprefix $(BIN-DIR)/, $(SRC-DIRS))

# Rule to create all programs
all: objects programs

programs: $(PROGRAMS)

objects: $(OBJECTS)

# Rule to link each binary from corresponding object files
define make_binary_rule
$(BIN-DIR)/$(1): $$(filter $(OBJ-DIR)/$(1)/%.o, $(OBJECTS))
	@mkdir -p $$(BIN-DIR)
	$$(CXX) $$^ -o $$@ $$(CXXFLAGS)
endef

# Rule to compile each source file into object files
$(OBJ-DIR)/%.o: $(SRC-PARENT-DIR)/%.cpp
	@mkdir -p $(OBJ-DIR)
	@mkdir -p $(foreach srcDir, $(SRC-DIRS), $(OBJ-DIR)/$(srcDir))
	$(CXX) -o $@ -c $< $(CXXFLAGS)

# Apply the rule for each program
$(foreach program,  $(SRC-DIRS), $(eval $(call make_binary_rule,$(program))))

clean:
	rm -rf $(OBJ-DIR) $(BIN-DIR)

.PHONY: all clean
