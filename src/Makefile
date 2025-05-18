GCC = gcc
CFLAGS = -c -std=c11 -Wall -Werror -Wextra
LFLAGS = -lm -lcheck  # -g -fsanitize=address
SOURCE = s21_*.c
STATIC_LIB = s21_matrix.a
BINARY = s21_test
OBJECTS = $(patsubst %.c,%.o, *.o)
OS := $(shell uname -s)

ifeq ($(OS), Linux)
	LFLAGS = -lm -lcheck -lsubunit #-g -fsanitize=address
endif

all: $(STATIC_LIB)

$(STATIC_LIB):
	$(GCC) $(CFLAGS) $(SOURCE) 
	ar rc $@ $(OBJECTS)
	ranlib $@
	cp $@ lib$@

test: $(STATIC_LIB)
	$(GCC) -o $(BINARY) test.c $(STATIC_LIB) $(LFLAGS)
	./$(BINARY)

clang: 
	cp ../materials/linters/.clang-format .clang-format 
	clang-format -i *.c *.h 
	rm -rf .clang-format

gcov_report: 
	$(GCC) --coverage $(SOURCE) test.c -o $(BINARY) $(LFLAGS)
	./$(BINARY)
	lcov -t "s21_test" -o s21_test.info -c -d .
	genhtml -o report s21_test.info
	open ./report/index.html
	rm -rf *.gcno *.gcda *.info $(BINARY)
clean:
	rm -rf *.o *.a $(STATIC_LIB) $(BINARY) report *.dSYM *.gcno *.gcda
