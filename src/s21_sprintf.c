#include "s21_sprintf.h"

const char *get_flag(const char *format, Znach *znach) {
  while (format) {
    if (*format == '-') {
      znach->minus = ON;
    } else if (*format == '+') {
      znach->plus = ON;
      znach->sign = ON;
    } else if (*format == '0') {
      znach->zero = ON;
      znach->zero_c = ON;
    } else if (*format == ' ') {
      znach->spase = ON;
      znach->sign = ON;
    } else if (*format == '#') {
      znach->hash = ON;
    } else {
      break;
    }
    format++;
  }
  if (znach->plus && znach->spase) {
    znach->spase = OFF;
  }
  if (znach->minus && znach->zero) {
    znach->zero = OFF;
  }
  return format;
}

const char *get_num(const char *format, int *shirina, va_list *args) {
  *shirina = 0;
  while (*format) {
    if (NOL <= *format && *format <= (NOL + 9)) {
      *shirina = *shirina * 10;
      *shirina = *shirina + (*format - NOL);
    } else
      break;
    format++;
  }
  if (*format == '*') {
    *shirina = va_arg(*args, int);
    format++;
  }
  return format;
}

const char *get_tochnost(const char *format, Znach *znach, va_list *args) {
  if (*format == '.') {
    znach->dot = ON;

    znach->zero = OFF;

    format++;
    format = get_num(format, &znach->tochnost, args);
  }
  return format;
}

const char *get_dlina(const char *format, Znach *znach) {
  if (*format == 'l') {
    znach->dlina = 'l';
    format++;
  } else if (*format == 'L') {
    znach->dlina = 'L';
    format++;
  } else if (*format == 'h') {
    znach->dlina = 'h';
    format++;
  }
  return format;
}

const char *set_znach(const char *format, Znach *znach, va_list *args) {
  format = get_flag(format, znach);
  format = get_num(format, &znach->shirina, args);
  format = get_tochnost(format, znach, args);
  format = get_dlina(format, znach);

  return format;
}

const char *check_spec(const char *format, Znach *znach) {
  int activate = 0;
  while (*format) {
    if (*format == 'c') {
      znach->c = ON;
    } else if (*format == 'd' || *format == 'i') {
      znach->d = ON;
    } else if (*format == 'f') {
      znach->f = ON;
    } else if (*format == 's') {
      znach->s = ON;
    } else if (*format == 'u') {
      znach->u = ON;
    } else if (*format == 'g') {
      znach->g = ON;
    } else if (*format == 'G') {
      znach->G = ON;
    } else if (*format == 'e') {
      znach->e = ON;
    } else if (*format == 'E') {
      znach->E = ON;
    } else if (*format == 'x') {
      znach->x = ON;
    } else if (*format == 'X') {
      znach->X = ON;
    } else if (*format == 'o') {
      znach->o = ON;
    } else if (*format == 'p') {
      znach->p = ON;
    } else if (*format == '%') {
      znach->proc = ON;
    }
    if (znach->p == ON || znach->o == ON || znach->X == ON || znach->x == ON ||
        znach->E == ON || znach->e == ON || znach->c == ON || znach->d == ON ||
        znach->f == ON || znach->s == ON || znach->u == ON || znach->g == ON ||
        znach->G == ON || znach->proc == ON)
      activate = ON;
    if (activate)
      break;
    else
      format++;
  }
  return format;
}

s21_size_t check_d_size(Znach *znach, long int num) {
  long int c_num = num;
  s21_size_t size = 0;
  if (num == 0) size++;
  if (num < 0) c_num = c_num * (-1);
  while (c_num > 0) {
    c_num = c_num / 10;
    size++;
    znach->var++;
  }
  if ((s21_size_t)znach->shirina > size) size = znach->shirina;
  if ((s21_size_t)znach->tochnost > size) size = znach->tochnost;

  if (num < 0 || znach->plus || znach->spase) size++;
  return size;
}

char num_to_char(long int c_num, Znach znach) {
  char tmp = 0;
  if (c_num >= 0 && c_num <= 9) tmp = (char)(c_num + NOL);
  if (c_num >= 10 && c_num <= 15)
    tmp = (char)(c_num + FOR_CHAR - (znach.upper_case * UP));
  return tmp;
}

int obrabotka_d_znach(Znach znach, s21_size_t *size, char *buf, int neg,
                      int *x) {
  if (znach.tochnost > (*x) &&
      (!znach.e || !znach.E || !znach.g || !znach.G || !znach.f)) {
    for (int i = znach.tochnost < (int)(*size) ? znach.tochnost - (znach.var)
                                               : znach.tochnost - (*x);
         i > 0; i--) {
      buf[(*x)] = '0';
      (*x)++;
      (*size)--;
    }
  }
  while ((znach.zero_c && !znach.minus) && (((*size) - znach.sign) > 0)) {
    buf[(*x)] = '0';
    (*x)++;
    (*size)--;
  }
  if (znach.spase && !neg && !znach.plus) {
    buf[(*x)] = ' ';
    (*x)++;
    (*size)--;
  } else if (neg) {
    buf[(*x)] = '-';
    (*x)++;
    (*size)--;
  } else if (znach.plus && znach.sign) {
    buf[(*x)] = '+';
    (*x)++;
    (*size)--;
  }
  if ((*size) > 0 && znach.minus == OFF) {
    while (((*size) - znach.sign > 0) && buf && ((*size) > 0)) {
      buf[(*x)] = ' ';
      (*x)++;
      (*size)--;
    }
  }
  return (*x);
}

int dec_to_string(Znach *znach, long int num, char *buf, s21_size_t size) {
  long int c_num = num;
  int neg = num < 0 ? 1 : 0;
  if (neg) {
    c_num = c_num * (-1);
    znach->sign = ON;
  }
  int x = 0;
  if (num == 0) {
    char sym = NOL;
    buf[x] = sym;
    x++;
    size--;
  }
  while (c_num && buf && size) {
    char sym = num_to_char(c_num % 10, *znach);
    buf[x] = sym;
    x++;
    size--;
    c_num = c_num / 10;
  }
  x = obrabotka_d_znach(*znach, &size, buf, neg, &x);
  return x;
}

char *print_to_str(char *str, int x, char *buf, Znach znach) {
  for (int i = x - 1; i >= 0; i--) {
    *str = buf[i];
    str++;
  }
  while ((znach.shirina - x > 0) && znach.zero == OFF) {
    *str = ' ';
    str++;
    x++;
  }
  return str;
}

char *print_d(char *str, Znach znach, va_list *args) {
  long int num = 0;
  if (znach.dlina == 'l') {
    num = (long int)va_arg(*args, long);
  } else if (znach.dlina == 'h') {
    num = (short)va_arg(*args, int);
  } else {
    num = (int)va_arg(*args, int);
  }

  s21_size_t size = check_d_size(&znach, num);
  char *buf = malloc(sizeof(char) * size);
  if (buf) {
    int x = dec_to_string(&znach, num, buf, size);
    str = print_to_str(str, x, buf, znach);
  }
  if (buf) free(buf);
  return str;
}

s21_size_t check_unsig_size(Znach *znach, unsigned long int num) {
  unsigned long int c_num = num;
  s21_size_t size = 0;
  if (num == 0) size++;
  while (c_num > 0) {
    c_num = c_num / znach->num_system;
    size++;
    znach->var++;
  }
  if ((s21_size_t)znach->shirina > size) size = znach->shirina;
  if ((s21_size_t)znach->tochnost >= size) {
    size = znach->tochnost;
  }
  if (znach->hash && (znach->u == OFF)) {
    size++;
    if (znach->X || znach->x || znach->p) size++;
  }

  return size;
}

int obrabotka_unsig_znach(Znach znach, s21_size_t *size, char *buf, int sys,
                          int x) {
  if (znach.tochnost > x) {
    int i = 0;
    if (znach.tochnost > znach.shirina) i++;
    for (i += znach.tochnost < (int)(*size) ? znach.tochnost - (znach.var - 1)
                                            : (int)(*size);
         i > (1 + (sys * znach.hash)); i--) {
      buf[x] = '0';
      x++;
      (*size)--;
    }
  }
  while (znach.zero && (((*size) - (sys * znach.hash)) > 0)) {
    buf[x] = '0';
    x++;
    (*size)--;
  }
  if (znach.sign == OFF) {
    if (znach.hash && (sys == 2) && znach.upper_case) {
      buf[x] = 'X';
      x++, (*size)--;
      buf[x] = '0';
      x++, (*size)--;
    }
    if (znach.hash && (sys == 2) && !znach.upper_case) {
      buf[x] = 'x';
      x++, (*size)--;
      buf[x] = '0';
      x++, (*size)--;
    } else if (znach.hash && (sys == 1)) {
      buf[x] = '0';
      x++;
      (*size)--;
    }
    if ((*size) > 0 && znach.minus == OFF)
      while (((*size) - (sys * znach.hash) > 0) && buf && ((*size) > 0)) {
        buf[x] = ' ';
        x++;
        (*size)--;
      }
  } else if ((*size) > 0 && znach.minus == OFF) {
    while (buf && ((*size) > 0)) {
      buf[x] = ' ';
      x++;
      (*size)--;
    }
  }
  return x;
}

int unsig_to_string(Znach *znach, unsigned long int num, char *buf,
                    s21_size_t size) {
  long int c_num = num;
  int sys = 0;
  if (znach->num_system != 10) sys = znach->num_system == 16 ? 2 : 1;
  int x = 0;
  if (num == 0) {
    char sym = NOL;
    buf[x] = sym;
    x++;
    size--;
    znach->sign = ON;
  }
  while (c_num && buf && size) {
    char sym = num_to_char(c_num % znach->num_system, *znach);
    buf[x] = sym;
    x++;
    size--;
    c_num = c_num / znach->num_system;
  }
  x = obrabotka_unsig_znach(*znach, &size, buf, sys, x);

  return x;
}

char *print_Xuxo(char *str, Znach znach, va_list *args) {
  unsigned long int num = 0;
  if (znach.dlina == 'l') {
    num = (unsigned long int)va_arg(*args, unsigned long);
  } else if (znach.dlina == 'h') {
    num = (unsigned short)va_arg(*args, unsigned int);
  } else {
    num = (unsigned int)va_arg(*args, unsigned int);
  }

  s21_size_t size = check_unsig_size(&znach, num);
  char *buf = malloc(sizeof(char) * size);
  if (buf) {
    int x = unsig_to_string(&znach, num, buf, size);
    str = print_to_str(str, x, buf, znach);
  }
  if (buf) free(buf);
  return str;
}

char *print_c(char *str, Znach znach, va_list *args) {
  char *ptr = S21_NULL;
  int sym = 0;
  if (znach.proc == ON && znach.c == OFF)
    sym = '%';
  else
    sym = va_arg(*args, int);
  int x = 0;
  if (znach.minus == OFF) {
    if (znach.zero_c == ON) {
      for (int i = znach.shirina; i - 1 > 0; i--) {
        *str = '0';
        str++;
        x++;
      }
    } else {
      for (int i = znach.shirina; i - 1 > 0; i--) {
        *str = ' ';
        str++;
        x++;
      }
    }
  }
  if (sym <= ASCII) {
    *str = sym;
    str++;
    x++;
    if (znach.minus == ON) {
      for (int i = znach.shirina; i - 1 > 0; i--) {
        *str = ' ';
        str++;
        x++;
      }
    }
    ptr = str;
  }
  return ptr;
}

char *print_s(char *str, Znach znach, va_list *args) {
  char *ptr = str;
  char *string = va_arg(*args, char *);
  if (string) {
    int shirina = znach.shirina;
    int x = 0;
    s21_size_t s_shir = s21_strlen(string);
    if ((s21_size_t)znach.shirina < s_shir) {
      znach.shirina = s_shir;
    }
    int spase = znach.shirina - s_shir;
    if (znach.tochnost == OFF) znach.tochnost = znach.shirina;
    if (znach.tochnost != OFF && znach.tochnost < shirina)
      spase = shirina - znach.tochnost;
    for (; spase && !znach.minus; spase--) {
      *str = ' ';
      str++;
    }
    for (int i = znach.tochnost; *string != '\0' && i > 0; i--) {
      *str = *string;
      str++;
      string++;
      x++;
    }
    for (; spase; spase--) {
      *str = ' ';
      str++;
    }
  } else {
    str = s21_memcpy(str, "(null)", 6);
    str += 6;
  }
  if (ptr) ptr = str;
  return ptr;
}

char *print_p(char *str, Znach znach, va_list *args) {
  unsigned long int ptr = (unsigned long)va_arg(*args, unsigned long);

  s21_size_t size = check_unsig_size(&znach, ptr);
  char *buf = malloc(sizeof(char) * size);
  if (buf) {
    int x = unsig_to_string(&znach, ptr, buf, size);
    str = print_to_str(str, x, buf, znach);
  }
  if (buf) free(buf);
  return str;
}

s21_size_t check_f_size(Znach *znach, long double num) {
  long double c_num = num;
  s21_size_t size = 0;
  if (num == 0) size++;
  int neg = num < 0 ? 1 : 0;
  if (neg) {
    c_num = c_num * (-1);
    znach->sign = ON;
    size++;
  }
  modfl(c_num, &c_num);
  if (c_num == 0) size++;
  while (c_num > 0) {
    c_num = c_num / znach->num_system;
    modfl(c_num, &c_num);
    size++;
  }
  if (znach->dot == OFF) znach->tochnost = 6;
  size += znach->tochnost;
  if (znach->tochnost) size++;
  if ((s21_size_t)znach->shirina > size) size = znach->shirina;
  return size;
}

s21_size_t sym_to_str(char *buf, int *x, char ch) {
  s21_size_t ret = 0;
  buf[(*x)] = ch;
  ret++;
  (*x)++;
  return ret;
}

char *e_to_str(char *buf, int poww, s21_size_t *size, Znach znach, int *x) {
  int c_pow = poww;
  if (c_pow == 0) *size -= sym_to_str(buf, x, NOL);
  while (c_pow > 0) {
    *size -= sym_to_str(buf, x, num_to_char(c_pow % znach.num_system, znach));
    c_pow = c_pow / 10;
  }
  if (poww < 10) *size -= sym_to_str(buf, x, NOL);
  if (znach.var == 1 || poww == 0)
    *size -= sym_to_str(buf, x, '+');
  else
    *size -= sym_to_str(buf, x, '-');
  if (znach.upper_case)
    *size -= sym_to_str(buf, x, 'E');
  else
    *size -= sym_to_str(buf, x, 'e');
  return buf;
}

s21_size_t dble_to_char(long double osnova, long double drob, Znach *znach,
                        char *buf, s21_size_t size, int *x, int neg) {
  while (drob == 0 &&
         (((znach->g || znach->G) && znach->tochnost) ||
          (!znach->g && !znach->G)) &&
         (((znach->f) && znach->tochnost) || (!znach->f)) &&
         (((znach->e || znach->E) && znach->tochnost) ||
          (!znach->e && !znach->E))) {
    char sym = NOL;
    buf[(*x)] = sym;
    (*x)++;
    size--;
    znach->tochnost--;
    znach->flag = ON;
  }

  int c_size = size - znach->tochnost;
  while (drob && buf && ((size - c_size) > 0)) {
    char sym = num_to_char((long)drob % znach->num_system, *znach);
    buf[(*x)] = sym;
    (*x)++;
    size--;
    drob = drob / znach->num_system;
  }

  if (((znach->e || znach->E) && (znach->tochnost || znach->flag)) ||
      ((znach->g || znach->G) && (znach->tochnost || znach->flag)) ||
      (znach->f && (znach->tochnost || znach->flag || znach->shirina))) {
    buf[(*x)] = '.';
    size--, (*x)++;
  }
  if (osnova < 1 && (!znach->g || !znach->G)) {
    char sym = NOL;
    buf[(*x)] = sym;
    (*x)++, size--;
  } else if ((osnova < 1) && (znach->G || znach->g)) {
    for (char sym = NOL; (znach->shirina - (*x)) > 0; (*x)++, size--) {
      buf[(*x)] = sym;
    }
  }
  while ((osnova >= 1) && buf && size) {
    char sym = num_to_char((int)osnova % znach->num_system, *znach);
    buf[(*x)] = sym;
    (*x)++;
    size--;
    osnova = osnova / znach->num_system;
  }
  (*x) = obrabotka_d_znach(*znach, &size, buf, neg, x);
  return size;
}

int dble_to_string(Znach *znach, long double num, char *buf, s21_size_t size,
                   int *poww) {
  int x = 0;
  int neg = num < 0 ? 1 : 0;
  num = neg == 1 ? num * (-1) : num;
  long double osnova = 0;
  long double drob = modfl(num, &osnova);
  if (znach->E || znach->e || ((znach->g || znach->G) && (znach->var > 0))) {
    buf = e_to_str(buf, *poww, &size, *znach, &x);
    if (num >= 1) osnova = num / pow(10, (double)*poww);
    if (num < 1) osnova = num * pow(10, (double)*poww);
    drob = osnova;
  }
  if ((znach->g || znach->G) && (znach->var) && znach->dot) znach->tochnost--;
  for (int i = 0; i < (znach->tochnost + 1); i++) {
    drob *= 10;
  }
  if (((long)drob % 10) > 4)
    if (!znach->tochnost)
      osnova++;
    else
      drob = (drob / 10) + 1;
  else
    drob = (drob / 10);

  if ((drob == 0) && (znach->g || znach->G)) {
    size -= znach->tochnost + 1;
    znach->tochnost = 0;
  }
  if ((drob > 0) && (znach->g || znach->G)) {
    while ((long)drob % 10 == 0 && znach->tochnost > 0 && (long)drob != 0) {
      drob = drob / 10;
      if (znach->var) size--;
      znach->tochnost--;
    }
  }

  size = dble_to_char(osnova, drob, znach, buf, size, &x, neg);
  if (size > 0 && znach->minus == OFF) {
    while ((size > 0) && buf) {
      if ((znach->g || znach->G) && (znach->dlina < znach->tochnost)) break;
      buf[x] = ' ';
      x++;
      size--;
    }
  }
  return x;
}

char *print_f(char *str, Znach znach, va_list *args) {
  long double num = 0;
  if (znach.dlina == 'L')
    num = va_arg(*args, long double);
  else
    num = va_arg(*args, double);
  s21_size_t size = check_f_size(&znach, num);
  char *buf = malloc(sizeof(char) * size);
  if (buf) {
    int x = dble_to_string(&znach, num, buf, size, 0);
    str = print_to_str(str, x, buf, znach);
  }
  if (buf) free(buf);
  return str;
}

long double e_thing(long double *num, Znach *znach) {
  int o = 0;
  if (fabsl(*num) > 1) {
    while (fabsl(*num) > 10) {
      *num = *num / 10;
      o++;
      znach->var = 1;
    }
  } else {
    while (fabsl(*num) < 0.999999) {
      if (*num != 0) {
        *num = *num * 10;
        o++;
        znach->var = 2;
      } else {
        znach->var = 1;
        break;
      }
    }
  }
  return o;
}

Znach g_thing(long double num, Znach *znach, int *poww) {
  Znach *c_znach = znach;
  long double c_num = num;
  (*poww) = e_thing(&c_num, c_znach);
  if (((*poww) <= 4 && c_znach->var == 2) ||
      (c_znach->var == 1 && (*poww) <= 5)) {
    c_znach->var = 0;
  }

  return *c_znach;
}

s21_size_t check_e_size(Znach *znach, long double num, int *poww) {
  s21_size_t size = 0;
  int neg = num < 0 ? 1 : 0;
  if (neg) {
    znach->sign = ON;
    size++;
  }
  *poww = e_thing(&num, znach);
  if (*poww < 100)
    size += 5;
  else
    size += 6;
  if (znach->dot == OFF) znach->tochnost = 6;
  size += znach->tochnost;
  if (znach->tochnost) size++;
  if ((s21_size_t)znach->shirina > size) size = znach->shirina;

  return size;
}

s21_size_t check_g_size(Znach *znach, long double num, int *poww) {
  s21_size_t size = 0;
  long double c_num = num;
  if (num == 0) size++;
  int neg = num < 0 ? 1 : 0;
  if (neg) {
    c_num = c_num * (-1);
    znach->sign = ON;
    size++;
  }
  *znach = g_thing(num, znach, poww);
  if (znach->var) {
    if (znach->dot == OFF) {
      znach->tochnost = 5;
    }
    size += znach->tochnost;
    if ((*poww) < 100) {
      size += 5;
    } else {
      size += 6;
    }
    if ((s21_size_t)znach->shirina > size) size = znach->shirina;
  } else {
    modfl(c_num, &c_num);
    while (c_num > 0) {
      c_num = c_num / znach->num_system;
      modfl(c_num, &c_num);
      size++;
    }
    if ((znach->dot == OFF) && ((s21_size_t)znach->shirina < size))
      znach->shirina = 6;
    if (znach->tochnost == OFF) {
      if (znach->shirina > 6)
        znach->tochnost = 6 - size;
      else
        znach->tochnost = znach->shirina - size;
    } else {
      znach->tochnost -= size;
    }
    size += znach->tochnost;
    if (znach->tochnost) size++;
    if ((s21_size_t)znach->shirina > size) {
      size = znach->shirina;
      if ((s21_size_t)znach->tochnost > size) size++;
    }
    if ((s21_size_t)znach->tochnost > size) size = znach->tochnost;
  }
  return size;
}

char *print_g(char *str, Znach znach, va_list *args) {
  long double num = 0;
  int poww = 0;
  if (znach.dlina == 'L')
    num = va_arg(*args, long double);
  else
    num = va_arg(*args, double);

  s21_size_t size = check_g_size(&znach, num, &poww);
  char *buf = malloc(sizeof(char) * size);
  if (buf) {
    int x = dble_to_string(&znach, num, buf, size, &poww);
    str = print_to_str(str, x, buf, znach);
  }
  if (buf) free(buf);
  return str;
}

char *print_e(char *str, Znach znach, va_list *args) {
  long double num = 0;
  int poww = 0;
  if (znach.dlina == 'L')
    num = va_arg(*args, long double);
  else
    num = va_arg(*args, double);

  s21_size_t size = check_e_size(&znach, num, &poww);
  char *buf = malloc(sizeof(char) * size);
  if (buf) {
    int x = dble_to_string(&znach, num, buf, size, &poww);
    str = print_to_str(str, x, buf, znach);
  }
  if (buf) free(buf);
  return str;
}

char *print_str(char *str, Znach znach, va_list *args) {
  if (znach.c) str = print_c(str, znach, args);
  if (znach.d) str = print_d(str, znach, args);
  if (znach.f) str = print_f(str, znach, args);
  if (znach.s) str = print_s(str, znach, args);
  if (znach.e || znach.E) str = print_e(str, znach, args);
  if (znach.G || znach.g) str = print_g(str, znach, args);
  if (znach.X || znach.u || znach.x || znach.o)
    str = print_Xuxo(str, znach, args);
  if (znach.p) str = print_p(str, znach, args);
  if (znach.proc) str = print_c(str, znach, args);
  if (!znach.c && !znach.d && !znach.f && !znach.s && !znach.e && !znach.E &&
      !znach.G && !znach.g && !znach.X && !znach.u && !znach.x && !znach.o &&
      !znach.p && !znach.proc)
    str = S21_NULL;
  return str;
}

void check_system(Znach *znach) {
  znach->num_system = 10;
  if (znach->X || znach->G || znach->E) znach->upper_case = ON;
  if (znach->o) znach->num_system = 8;
  if (znach->x || znach->X) znach->num_system = 16;
  if (znach->p) {
    znach->num_system = 16;
    znach->hash = ON;
  }
  if (znach->x || znach->X || znach->u || znach->o) znach->sign = OFF;
}

int s21_sprintf(char *str, const char *format, ...) {
  va_list(args);
  va_start(args, format);
  char *src = str;
  while (*format) {
    if (*format != '%') {
      *str = *format;
      str++;
    } else {
      format++;
      Znach znach = {0};
      format = set_znach(format, &znach, &args);
      format = check_spec(format, &znach);
      check_system(&znach);
      str = print_str(str, znach, &args);
    }
    format++;
  }

  *str = '\0';
  va_end(args);
  return (str - src);
}
