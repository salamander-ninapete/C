#ifndef Grep_C
#define Grep_C

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct arguments {
  int c, e, l, v, i, n, s, h, f, o;
  char pattern[1024];  // убрали звездочку!

  int len_pattern;
  // int mem_pattern;

} arguments;

void prin_match(arguments arg, regex_t *re, char *line, char *path);
void pattern_add(arguments *arg, char *pattern);
void add_reg_from_file(arguments *arg, char *filepath);
arguments arguments_parser(int argc, char *argv[]);
void output_line(char *line, int n);
void processFile(arguments arg, char *path, regex_t *reg);
void output(arguments arg, int argc, char **argv);
#endif