SHELL=/bin/sh
GCC = gcc
FLAGS = -std=c11 -Wall -Werror -Wextra -lncurses
T_FLAGS = -lcheck -lm -lncurses
BACK_DIR = brick_game/tetris
FRONT_DIR = gui/cli
TEST_DIR = test
BACK_SRCS = $(wildcard $(BACK_DIR)/*.c)
FRONT_SRCS = $(wildcard $(FRONT_DIR)/*.c)
CHECK = checkmk 
CHECK_SRSC = $(wildcard $(TEST_DIR)/*.check)
NAME = Tetris
VERSION = _v0.1
DELETE = rm -rf

# Директория для выходных файлов
OUTPUT_DIR = TETRIS

# Целевой исполняемый файл с указанием директории
TARGET_PATH = $(OUTPUT_DIR)/$(NAME)$(VERSION)

.PHONY: all install start clang clang_test test dvi dist uninstall clean gcov_report

all: install start

install: $(TARGET_PATH)

$(TARGET_PATH): $(BACK_SRCS) $(FRONT_SRCS) | $(OUTPUT_DIR)
	$(GCC) $(FLAGS) $(BACK_SRCS) $(FRONT_SRCS) -o $@

$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR)

start: install
	./$(TARGET_PATH)

clang:
	cp ../materials/linters/.clang-format .clang-format
	clang-format -i $(BACK_SRCS) $(FRONT_SRCS)
	$(DELETE) .clang-format

clang_test:
	cp ../materials/linters/.clang-format .clang-format
	clang-format -n $(BACK_SRCS) $(FRONT_SRCS)
	$(DELETE) .clang-format

test:
	$(CHECK) $(CHECK_SRSC) > test/test.c
	$(CHECK) clean_mode=1 $(CHECK_SRSC) > test/test.c
	$(GCC) $(BACK_SRCS) test/test.c $(T_FLAGS) -o test_file
	./test_file

gcov_report:
	$(CHECK) $(CHECK_SRSC) > test/test.c
	$(CHECK) clean_mode=1 $(CHECK_SRSC) > test/test.c
	$(GCC) --coverage $(BACK_SRCS) test/test.c $(T_FLAGS) -o test_file
	./test_file
	lcov -t "tetris_test" -o tetris_test.info -c -d .
	genhtml -o report tetris_test.info
	open ./report/index.html
	$(DELETE) *.gcno *.gcda *.info test_file

dvi:
	@echo "Building HTML documentation..."
	@if [ ! -f "misc/docs.html" ]; then \
		echo "No such file!"; \
	else \
		if [ ! -d "docs" ]; then mkdir docs; fi; \
		cp misc/docs.html docs/docs.html; \
		echo "HTML documentation copied successfully."; \
		open docs/docs.html;  \
	fi

dist: clean
	@echo "Creating distribution package..."
	mkdir -p dist
	cp -r brick_game gui test Makefile misc dist/
	tar -czf $(NAME)$(VERSION).tar.gz dist
	$(DELETE) dist
	@echo "Distribution package created: $(NAME)$(VERSION).tar.gz"

uninstall:
	$(DELETE) $(TARGET_PATH)

clean:
	$(DELETE) $(OUTPUT_DIR) *.txt dist test_file *.gcno *.gcda *.gcov *.info report docs *.gz test/test.c