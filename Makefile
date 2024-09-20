# Compiler name
CC = gcc

# Compilation options for GTK 4
GTK_CFLAGS = $(shell pkg-config --cflags gtk4)
GTK_LIBS = $(shell pkg-config --libs gtk4)

# General compilation options
CFLAGS = -Iincludes -Wall -Wextra -g $(GTK_CFLAGS)

# Directories
SRC_DIR = src
GAME_DIR = $(SRC_DIR)/game
NETWORK_DIR = $(SRC_DIR)/network
TEST_SRC_DIR = $(SRC_DIR)/test
INCLUDES_DIR = includes
BUILD_DIR = build
DOCS_DIR = docs
TEST_BUILD_DIR = tests

# List of object files
OBJS = $(BUILD_DIR)/board.o $(BUILD_DIR)/gameLogic.o $(BUILD_DIR)/localMain.o $(BUILD_DIR)/gui.o $(BUILD_DIR)/ai.o \
       $(BUILD_DIR)/server.o $(BUILD_DIR)/serverMain.o $(BUILD_DIR)/client.o $(BUILD_DIR)/clientMain.o

# List of tests object files
TEST_OBJS = $(TEST_BUILD_DIR)/testBoard.o $(TEST_BUILD_DIR)/testGameLogic.o $(TEST_BUILD_DIR)/testAI.o $(TEST_BUILD_DIR)/mainTest.o

# Default target
all: $(BUILD_DIR)/game $(TEST_BUILD_DIR)/test $(DOCS_DIR)/docs

# Compile the final executable with GTK 4 and output to build directory as "game"
$(BUILD_DIR)/game: $(OBJS) $(BUILD_DIR)/game.o
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/game $(OBJS) $(BUILD_DIR)/game.o $(GTK_LIBS)

# Compilation of object files
$(BUILD_DIR)/%.o: $(GAME_DIR)/%.c $(INCLUDES_DIR)/%.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(NETWORK_DIR)/%.c $(INCLUDES_DIR)/%.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/game.o: $(SRC_DIR)/game.c $(INCLUDES_DIR)/game.h
	$(CC) $(CFLAGS) -c $< -o $@

# Tests: Compile test files and output to tests directory as "test"
$(TEST_BUILD_DIR)/test: $(TEST_OBJS) $(BUILD_DIR)/board.o $(BUILD_DIR)/gameLogic.o $(BUILD_DIR)/ai.o $(TEST_BUILD_DIR)/mainTest.o
	$(CC) $(CFLAGS) -o $(TEST_BUILD_DIR)/test $(TEST_OBJS) $(BUILD_DIR)/board.o $(BUILD_DIR)/gameLogic.o $(BUILD_DIR)/ai.o

$(TEST_BUILD_DIR)/%.o: $(TEST_SRC_DIR)/%.c $(INCLUDES_DIR)/%.h $(INCLUDES_DIR)/testsMacro.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_BUILD_DIR)/mainTest.o: $(TEST_SRC_DIR)/mainTest.c
	$(CC) $(CFLAGS) -c $< -o $@

# Documentation target: create "docs" executable in docs directory to launch the documentation
$(DOCS_DIR)/docs:
	echo "#!/bin/bash\n\
cd $(DOCS_DIR) && doxygen Doxyfile\n\
if [[ \`uname\` == \"Darwin\" ]]; then\n\
  open html/index.html\n\
else\n\
  xdg-open html/index.html\n\
fi" > $(DOCS_DIR)/docs
	chmod +x $(DOCS_DIR)/docs

# Clean object files, tests, the executables, and the documentation
clean:
	rm -f $(OBJS) $(BUILD_DIR)/game.o $(BUILD_DIR)/game $(TEST_OBJS) $(TEST_BUILD_DIR)/test_runner
	rm -rf $(DOCS_DIR)/html $(DOCS_DIR)/latex
	if [ -f $(TEST_BUILD_DIR)/test ]; then rm $(TEST_BUILD_DIR)/test; fi
	if [ -f $(DOCS_DIR)/docs ]; then rm $(DOCS_DIR)/docs; fi
