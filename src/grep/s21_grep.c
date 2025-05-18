// #include <getopt.h>
// #include <regex.h>
// #include <stdio.h>
// #include <string.h>

// typedef struct arguments {
//   int c, e, l, v, i, n, s, h, f, o;
//   char pattern[1024];  // убрали звездочку!

//   int len_pattern;
//   // int mem_pattern;

// } arguments;

// void prin_match(arguments arg, regex_t *re, char *line, char *path);
// void pattern_add(arguments *arg, char *pattern);
// void add_reg_from_file(arguments *arg, char *filepath);
// arguments arguments_parser(int argc, char *argv[]);
// void output_line(char *line, int n);
// void processFile(arguments arg, char *path, regex_t *reg);
// void output(arguments arg, int argc, char **argv);

#include "s21_grep.h"

int main(int argc, char **argv) {
  arguments arg = arguments_parser(argc, argv);
  output(arg, argc, argv);
  return 0;
}

void prin_match(arguments arg, regex_t *re, char *line, char *path) {
  if (re == NULL || line == NULL) {
    // Проверка на нулевые указатели
    printf("Invalid regex or line pointer\n");
    return;
  }
  regmatch_t match;
  int offset = 0;
  while (1) {
    int result = regexec(re, line + offset, 1, &match, 0);
    if (result != 0) {
      break;
    }
    if (!arg.h) printf("%s:", path);
    for (int i = match.rm_so; i < match.rm_eo; i++) {
      putchar(line[offset + i]);
    }
    putchar('\n');
    offset += match.rm_eo;
  }
}

void pattern_add(arguments *arg, char *pattern) {
  //  int n = strlen(pattern);
  //  if (pattern [n-1]== '\n') pattern[n-1]; переехало в на с 52

  if (arg->len_pattern != 0) {
    strcat(arg->pattern + arg->len_pattern, "|");
    arg->len_pattern++;
  }
  arg->len_pattern += sprintf(arg->pattern + arg->len_pattern, "(%s)", pattern);
}

void add_reg_from_file(arguments *arg, char *filepath) {
  FILE *f = fopen(filepath, "r");
  if (f == NULL) {
    if (!arg->s) perror(filepath);
    exit(1);
  }
  char *line = NULL;
  size_t memlen = 0;
  int read = getline(&line, &memlen, f);
  while (read != -1) {
    if (line[read - 1] == '\n') line[read - 1] = '\0';
    pattern_add(arg, line);
    read = getline(&line, &memlen, f);
  }
  free(line);
  fclose(f);
}

arguments arguments_parser(int argc, char *argv[]) {
  arguments arg = {0};
  int opt;
  while ((opt = getopt_long(argc, argv, "e:ivclnhsf:o", 0, 0)) != -1) {
    switch (opt) {
      case 'e':
        arg.e = 1;
        pattern_add(&arg, optarg);
        break;
      case 'i':
        arg.i = REG_ICASE;
        break;
      case 'v':
        arg.v = 1;
        break;
      case 'c':
        arg.c = 1;
        break;
      case 'l':
        arg.c = 1;
        arg.l = 1;
        break;
      case 'n':
        arg.n = 1;
        break;
      case 'h':
        arg.h = 1;
        break;
      case 's':
        arg.s = 1;
        break;
      case 'f':
        arg.f = 1;
        add_reg_from_file(&arg, optarg);
        break;
      case 'o':
        arg.o = 1;
        break;
      default:
        break;
    }
  }
  if (arg.len_pattern == 0) {
    pattern_add(&arg, argv[optind]);
    optind++;
  }
  if (argc - optind == 1) arg.h = 1;
  return arg;
}

void output_line(char *line, int n) {
  for (int i = 0; i < n; i++) {
    putchar(line[i]);
  }

  if (line[n - 1] != '\n') putchar('\n');
}

void processFile(arguments arg, char *path, regex_t *reg) {
  FILE *f = fopen(path, "r");
  if (f == NULL) {
    if (!arg.s) perror(path);
    exit(1);
  }
  char *line = NULL;
  size_t memlen = 0;
  int read = 0;
  int line_count = 1;
  int count = 0;
  read = getline(&line, &memlen, f);
  while (read != -1) {
    int result = regexec(reg, line, 0, NULL, 0);
    if ((result == 0 && !arg.v) || (arg.v && result != 0)) {
      if (!arg.c && !arg.l) {
        if (!arg.h && !arg.o) printf("%s:", path);
        if (arg.n) printf("%d:", line_count);
        if (arg.o) {
          prin_match(arg, reg, line, path);
        } else {
          output_line(line, read);
        }
      }
      count++;
    }
    read = getline(&line, &memlen, f);
    line_count++;
  }
  free(line);
  if (arg.c && !arg.l) {
    if (!arg.h) printf("%s:", path);
    printf("%d\n", count);
  }
  if (!arg.n && !arg.v && !arg.i && arg.c && arg.l && count > 0 && !arg.h) {
    printf("%s:", path);
  }
  if (!arg.i && !arg.v && !arg.n && arg.c && arg.l) {
    printf("1\n");
  }
  if (arg.l && count > 0) printf("%s\n", path);
  fclose(f);
}

void output(arguments arg, int argc, char **argv) {
  regex_t re;
  int error = regcomp(&re, arg.pattern, REG_EXTENDED | arg.i);
  if (error) perror("ERROR");
  for (int i = optind; i < argc; i++) {
    processFile(arg, argv[i], &re);
  }
  regfree(&re);
}

// c e l v i n s h  - (Ok)|| Ne OK ->  f o