#include "s21_string.h"

// Поиск 1-го вхождение символа в строке
char *s21_strchr(const char *str, int symbol) {
  char *result = S21_NULL;
  if (str != S21_NULL) {
    while (*str && *str != symbol) {
      str++;
    }
    if (*str == symbol) {
      result = (char *)str;
    }
  }
  return result;
}