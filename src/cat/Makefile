SHELL:=/bin/bash
G=gcc
W=-Wall -Werror -Wextra
Fi=test_1_cat.txt

all: s21_cat 
	 

s21_cat:
	$(G) $(W) s21_cat.c -o s21_cat 

cf:
	clang-format -i s21_cat.c 

test:
	$(G) $(W) s21_cat.c -o s21_cat 
	-diff <(./s21_cat -T $(Fi)) <(cat -t $(Fi))
	-diff <(./s21_cat --number  $(Fi)) <(cat -n $(Fi))
	-diff <(./s21_cat --number-nonblank $(Fi)) <(cat -b $(Fi))
	-diff <(./s21_cat --squeeze-blank $(Fi)) <(cat -s $(Fi))
	-diff <(./s21_cat -E $(Fi)) <(cat -e $(Fi))
clean: 
	rm -rf s21_cat

rebuild: clean cf all
