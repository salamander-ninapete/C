#ifndef S21_SPRINTF
#define S21_SPRINTF
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "s21_string.h"

#define NOL 48
#define FOR_CHAR 87
#define ASCII 127
#define UP 32
#define ON 1
#define OFF 0

typedef struct {
  int c;
  int d;
  int f;
  int s;
  int u;
  int g;
  int G;
  int e;
  int E;
  int x;
  int X;
  int o;
  int p;
  int proc;

  int plus;
  int minus;
  int spase;
  int zero;
  int hash;

  int shirina;
  int tochnost;
  char dlina;

  int zero_c;
  int num_system;
  int sign;
  int dot;
  int upper_case;
  int var;
  int flag;
} Znach;

s21_size_t check_d_size(Znach *znach, long int num);
s21_size_t check_unsig_size(Znach *znach, unsigned long int num);
s21_size_t check_f_size(Znach *znach, long double num);
s21_size_t check_e_size(Znach *znach, long double num, int *poww);
s21_size_t check_g_size(Znach *znach, long double num, int *poww);

s21_size_t sym_to_str(char *buf, int *x, char ch);
const char *get_flag(const char *format, Znach *znach);
const char *get_num(const char *format, int *shirina, va_list *args);
const char *get_tochnost(const char *format, Znach *znach, va_list *args);
const char *get_dlina(const char *format, Znach *znach);
const char *set_znach(const char *format, Znach *znach, va_list *args);
const char *check_spec(const char *format, Znach *znach);

char num_to_char(long int c_num, Znach znach);
int obrabotka_d_znach(Znach znach, s21_size_t *size, char *buf, int neg,
                      int *x);
int dec_to_string(Znach *znach, long int num, char *buf, s21_size_t size);
char *print_to_str(char *str, int x, char *buf, Znach znach);
char *print_d(char *str, Znach znach, va_list *args);
int obrabotka_unsig_znach(Znach znach, s21_size_t *size, char *buf, int sys,
                          int x);
int unsig_to_string(Znach *znach, unsigned long int num, char *buf,
                    s21_size_t size);
char *e_to_str(char *buf, int poww, s21_size_t *size, Znach znach, int *x);
s21_size_t dble_to_char(long double osnova, long double drob, Znach *znach,
                        char *buf, s21_size_t size, int *x, int neg);
int dble_to_string(Znach *znach, long double num, char *buf, s21_size_t size,
                   int *poww);
void check_system(Znach *znach);
long double e_thing(long double *num, Znach *znach);
Znach g_thing(long double num, Znach *znach, int *poww);

char *print_Xuxo(char *str, Znach znach, va_list *args);
char *print_c(char *str, Znach znach, va_list *args);
char *print_s(char *str, Znach znach, va_list *args);
char *print_p(char *str, Znach znach, va_list *args);
char *print_f(char *str, Znach znach, va_list *args);
char *print_g(char *str, Znach znach, va_list *args);
char *print_e(char *str, Znach znach, va_list *args);
char *print_str(char *str, Znach znach, va_list *args);

int s21_sprintf(char *str, const char *format, ...);

#endif  // S21_SPRINTF
