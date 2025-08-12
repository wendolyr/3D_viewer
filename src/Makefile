CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -Wpedantic -Wshadow -std=c++20

OBJ_DIR = ./obj
SRC_DIRS = controller model
BACK_SRC = $(wildcard $(addsuffix /*.cc, $(SRC_DIRS)))
BACK_OBJ = $(patsubst %.cc, $(OBJ_DIR)/%.o, $(BACK_SRC))

TEST_SRC = $(wildcard ./tests/*.cc)
TESTS_BIN = test
LOGS_DIR = ./logs

DIST_DIR = dist
DIST_NAME = 3D_viewer_v2.0.tar.gz
DIST_FILES = controller model view tests Makefile CmakeLists.txt Doxyfile *.cc

all: clean lib.a

install: uninstall
		@mkdir -p build
		@cd build && cmake ..
		@$(MAKE) -C build

lib.a: $(BACK_OBJ)
		@ar rcs $@ $^

uninstall: 
		@rm -rf build

run:
		@./build/viewer

test: clean $(BACK_OBJ)
		@$(CXX) $(CXXFLAGS) $(BACK_OBJ) $(TEST_SRC) -o $(TESTS_BIN) -lgtest
		@clear
		@./$(TESTS_BIN)

gcov_report: CXXFLAGS += --coverage
gcov_report: test
		@mkdir -p $(LOGS_DIR)
		@echo "Generating coverage report via gcovr..."
		@gcovr -r . \
		--exclude-unreachable-branches \
		--exclude 'tests/' \
		--exclude 'controller/' \
		--html-details \
		--html-theme green \
		-o $(LOGS_DIR)/coverage_report.html
		@echo "Report generated: coverage_report.html"
		@open $(LOGS_DIR)/coverage_report.html

dvi:
		@doxygen && open doxygen/html/index.html

dist:	clear
		@echo "Creating archive..."
		@mkdir -p $(DIST_DIR)
		@tar -czf $(DIST_DIR)/$(DIST_NAME) $(DIST_FILES)
		@echo "Archive created!"

leaks: test
		@leaks -atExit -- ./$(TESTS_BIN)

valgrind: test
		@valgrind --tool=memcheck --leak-check=yes ./$(TESTS_BIN)

clean:
		@rm -rf $(OBJ_DIR) $(TESTS_BIN) $(LOGS_DIR) *.gcno *.gcda .last_object.obj .last_settings.txt doxygen *.a *.out

$(OBJ_DIR)/%.o: %.cc
		@mkdir -p $(dir $@)
		@$(CXX) $(CXXFLAGS) -c $< -o $@