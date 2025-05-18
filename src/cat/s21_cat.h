#ifndef Cat_C
#define Cat_C
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int b;
  int n;
  int s;
  int v;
  int E;
  int T;
} Flags;

Flags Parsing(int argc, char **argv);
void outline(Flags *flags, char *line, int n);
char v_output(char ch);
void output(Flags *flags, char **argv, int argc);
#endif