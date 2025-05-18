// #include "getopt.h"
// #include "stdlib.h"
// #include "stdio.h"

// typedef struct {
//   int b;
//   int n;
//   int s;
//   int v;
//   int E;
//   int T;
// } Flags;

// Flags Parsing(int argc, char *argv[], int *hasInputFile);
// void outline(Flags *flags, char *line, int n);
// char v_output(char ch);
// void output(Flags *flags, char *filename, int argc);

#include "s21_cat.h"

int main(int argc, char **argv) {
  Flags flags = Parsing(argc, argv);
  output(&flags, argv, argc);
  return 0;
}

Flags Parsing(int argc, char **argv) {
  int currentFlags = 0;
  int option_index = 0;
  static struct option long_options[] = {{"number-nonblank", 0, 0, 'b'},
                                         {"number", 0, 0, 'n'},
                                         {"squeeze-blank", 0, 0, 's'},
                                         {0, 0, 0, 0}};
  Flags flags = {0};
  for (; currentFlags != -1;
       currentFlags =
           getopt_long(argc, argv, "+vbnsetET", long_options, &option_index)) {
    switch (currentFlags) {
      case 'b':
        flags.b = 1;
        break;
      case 'n':
        flags.n = 1;
        break;
      case 's':
        flags.s = 1;
        break;
      case 'e':
        flags.E = 1;
        flags.v = 1;
        break;
      case 't':
        flags.T = 1;
        flags.v = 1;
        break;
      case 'E':
        flags.E = 1;
        break;
      case 'v':
        flags.v = 1;
        break;
      case 'T':
        flags.T = 1;
        break;
      case '?':
        perror("ERROR");
        exit(1);
        break;
      default:
        break;
    }
  }
  return flags;
}

void outline(Flags *flags, char *line, int n) {
  for (int i = 0; i < n; i++) {
    if (flags->T && line[i] == '\t')
      printf("^I");
    else {
      if (flags->E && line[0] == '\n' && flags->b) {
        printf("      \t");
        putchar('$');
      } else if (flags->E && line[i] == '\n') {
        putchar('$');
      }
      if (flags->v) {
        line[i] = v_output(line[i]);
      }
      putchar(line[i]);
    }
  }
}

char v_output(char ch) {
  if (ch == '\n' || ch == '\t') return ch;
  if (ch < 0) {
    printf("M-");
    ch = ch & 0x7f;
  }
  if (ch <= 31) {
    putchar('^');
    ch += 64;
  } else if (ch == 127) {
    putchar('^');
    ch = '?';
  }
  return ch;
}

void output(Flags *flags, char **argv, int argc) {
  for (int i = optind; i < argc; i++) {
    FILE *f = fopen(argv[i], "r");
    if (f != NULL) {
      char *line = NULL;
      size_t memline = 0;
      int line_count = 1;
      int empty_count = 0;
      int read = getline(&line, &memline, f);
      while (read != -1) {
        if (line[0] == '\n')
          empty_count++;
        else
          empty_count = 0;
        if (flags->s && empty_count > 1) {
          read = getline(&line, &memline, f);
        } else {
          if (flags->b && line[0] != '\n') {
            printf("%6i\t", line_count);
            line_count++;
          } else if (flags->n) {
            printf("%6i\t", line_count);
            line_count++;
          }
          outline(flags, line, read);
          read = getline(&line, &memline, f);
        }
      }
      if (line) free(line);
    }
    if (f) fclose(f);
  }
}
