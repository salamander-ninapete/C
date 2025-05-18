SHELL:=/bin/bash
G=gcc
W=-Wall -Werror -Wextra
Fi=1.txt 2.txt 
P=patterns.reg
CH=2>&1 valgrind --tool=memcheck --leak-check=yes --leak-check=full --show-leak-kinds=all


all: s21_grep 
	

s21_grep:
	$(G) $(W) s21_grep.c -o s21_grep 

cf:
	clang-format -i s21_grep.c 

test:
	$(G) $(W) s21_grep.c -o s21_grep 
	
	-diff <(./s21_grep He $(Fi)) <(grep He $(Fi))
	-diff <(./s21_grep -v He $(Fi)) <(grep -v He $(Fi))
	-diff <(./s21_grep -i He $(Fi)) <(grep -i He $(Fi))
	-diff <(./s21_grep -iv He $(Fi)) <(grep -iv He $(Fi))
	-diff <(./s21_grep -in He $(Fi)) <(grep -in He $(Fi))
	-diff <(./s21_grep -ic He $(Fi)) <(grep -ic He $(Fi))
	-diff <(./s21_grep -ie "^he" $(Fi)) <(grep -ie "^he" $(Fi))
	-diff <(./s21_grep -ince "^he" $(Fi)) <(grep -ince "^he" $(Fi))
	-diff <(./s21_grep -hc e $(Fi)) <(grep -hc e $(Fi))
	-diff <(./s21_grep -e se $(Fi)) <(grep -e se $(Fi))
	-diff <(./s21_grep -s se $(Fi)) <(grep -s se $(Fi))
	-diff <(./s21_grep -f $(P) $(Fi)) <(grep -f $(P) $(Fi))
	-diff <(./s21_grep -oi he $(Fi)) <(grep -oi he $(Fi))
	-diff <(./s21_grep -ohf $(P) $(Fi)) <(grep -ohf $(P) $(Fi))
	-diff <(./s21_grep -onic he $(Fi)) <(grep -onic he $(Fi))
clean:
	rm -rf s21_grep

rebuild: clean cf all
