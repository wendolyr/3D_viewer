CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -Wpedantic -Wshadow -std=c++20

OBJ_DIR = ./obj
SRC_DIRS = controller model
BACK_SRC = $(wildcard $(addsuffix /*.cc, $(SRC_DIRS)))
BACK_OBJ = $(patsubst %.cc, $(OBJ_DIR)/%.o, $(BACK_SRC))


all: install

$(OBJ_DIR)/%.o: %.cc
		@mkdir -p $(dir $@)
		@$(CXX) $(CXXFLAGS) -c $< -o $@

install: uninstall
	mkdir -p build
	cd build && cmake ..
	$(MAKE) -C build

test: clean $(BACK_OBJ)
		$(CXX) $(CXXFLAGS) $(BACK_OBJ) test.cc -o test
		clear
		./test

uninstall: 
	rm -rf build

run:
	./build/viewer

clean:
	rm -rf $(OBJ_DIR) test