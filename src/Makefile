CXX = g++
CXXFLAGS = -std=c++20 -Wall -Werror -Wextra

SRC_DIR = .
BUILD_DIR = build
TEST_BUILD_DIR = $(BUILD_DIR)/tests

APP_NAME = 3D_Viewer.app

COVFLAGS=-fprofile-arcs -ftest-coverage 

TEST_DIR = ./tests
C_TEST_FILES = $(shell find $(TEST_DIR) -type f -name "*.cpp")
O_TEST_FILES = $(patsubst $(TEST_DIR)/%.cpp,$(TEST_BUILD_DIR)/%.o,$(C_TEST_FILES))
OUT_TEST=$(TEST_BUILD_DIR)/test.out 
SRC_FILES = ./backend/affin.cpp ./backend/model.cpp ./backend/parser.cpp
O_FILES = $(patsubst ./backend/%.cpp,$(BUILD_DIR)/backend/%.o,$(SRC_FILES))
TARGET_TEST = funcs_test

# Флаги для тестов
TEST_FLAGS=-lgtest -lgtest_main
ifeq ($(CXX),clang)
    TEST_FLAGS+=-lc++ 
endif

OS=$(shell uname)
ifeq ($(OS),Darwin)
	OPENER = open
else
	OPENER = xdg-open
endif

$(BUILD_DIR)/%.o:: %.cpp
	$(CXX) $(CXXFLAGS) $(COVFLAGS) -c $< -o $@

$(TEST_BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(COVFLAGS) -c $< -o $@


.PHONY: all build run test install uninstall clean style dvi dist load

all: build

build:
	@echo "Building the project..."
	@mkdir -p $(TEST_BUILD_DIR)
	@mkdir -p $(BUILD_DIR)/backend
	@mkdir -p $(BUILD_DIR) && \
	cd $(BUILD_DIR) && \
	qmake ../3D_Viewer/3D_Viewer.pro && \
	make -j$(nproc)
	@echo "Build completed successfully!"

run: 
	@echo "Running the application..."
	@$(BUILD_DIR)/$(APP_NAME)/Contents/MacOS/$(APP_NAME:%.app=%) &

gcov_test: $(O_TEST_FILES) $(O_FILES)
	$(CXX) $(CXXFLAGS) $(COVFLAGS) $(O_TEST_FILES) $(O_FILES) -o $(OUT_TEST) $(TEST_FLAGS) 
	$(OUT_TEST)

test: clean build gcov_test
	@echo "Building and running tests..."
	lcov -t "$(TARGET_TEST)" -o $(TEST_BUILD_DIR)/$(TARGET_TEST).info -c -d $(BUILD_DIR) --ignore-errors mismatch,usage,inconsistent --exclude "*/backend/model.cpp" 
	genhtml -o $(TEST_BUILD_DIR)/gcov_report $(TEST_BUILD_DIR)/$(TARGET_TEST).info --exclude '/usr/*' --exclude 'viver.h'
	$(OPENER) ./$(TEST_BUILD_DIR)/gcov_report/index.html

install: build
	@echo "Installing to Desktop..."
	@rm -rf $(HOME)/Desktop/3DViewer_v2.0
	@mkdir -p $(HOME)/Desktop/3DViewer_v2.0/
	@cp -rf $(BUILD_DIR)/$(APP_NAME) $(HOME)/Desktop/3DViewer_v2.0/
	@echo "Installation complete!"

clean:
	@echo "Cleaning build artifacts..."
	@rm -rf $(BUILD_DIR) $(TEST_DIR)/tests doxygen latex html

uninstall:
	@echo "Uninstalling..."
	@rm -rf $(BUILD_DIR)*
	@rm -rf $(HOME)/Desktop/3DViewer_v2.0
	@echo "Uninstall complete!"

style:
	@echo "Checking code style..."
	@clang-format -n -style=Google \
	3D_Viewer/*.cpp \
	backend/*.cpp \
	tests/*.cpp 
	@echo "Applying code style..." 
	@clang-format -i -style=Google \
	3D_Viewer/*.cpp \
	backend/*.cpp \
	tests/*.cpp \

dvi:
	@echo "Generating documentation..."
	@/opt/goinfre/$(USER)/homebrew/bin/doxygen Doxyfile || (echo "Doxygen failed"; exit 1)
	@open doxygen/html/index.html

dist: build
	@echo "Creating distribution package..."
	@rm -f $(HOME)/Desktop/3DViewer_v2.0.tar.gz
	@tar -czf $(HOME)/Desktop/3DViewer_v2.0.tar.gz -C $(BUILD_DIR) $(APP_NAME)
	@echo "Distribution package created: $(HOME)/Desktop/3DViewer_v2.0.tar.gz"

load:
	@echo "Starting installation in goinfre..."
    
    # Create goinfre directory if not exists
	@if [ ! -d "/opt/goinfre/$(USER)" ]; then \
        echo "Creating goinfre directory..."; \
        mkdir -p /opt/goinfre/$(USER); \
    fi
    
    # Clone Homebrew
	@if [ ! -d "/opt/goinfre/$(USER)/homebrew" ]; then \
        echo "Cloning Homebrew..."; \
        git clone https://github.com/Homebrew/brew /opt/goinfre/$(USER)/homebrew; \
    else \
        echo "Homebrew already exists in goinfre, skipping clone..."; \
    fi
    
    # Set up Homebrew environment
	@echo "Setting up Homebrew environment..."
	@eval "$(/opt/goinfre/$(USER)/homebrew/bin/brew shellenv)"
    
    # Update Homebrew
	@echo "Updating Homebrew..."
	@brew update --force --quiet
    
    # Install required packages
	@echo "Installing packages..."
	@brew install lcov
	@brew install doxygen
	@brew install gotests
    
	@echo "Installation completed successfully!"