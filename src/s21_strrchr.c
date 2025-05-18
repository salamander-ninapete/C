#include "s21_string.h"

// Поиск последнего вхождения символа
char *s21_strrchr(const char *str, int symbol) {
  char *result = S21_NULL;
  for (; *str != '\0'; str++) {
    if (*str == symbol) {
      result = (char *)str;
    }
  }
  if (result == S21_NULL) {
    result = (char *)str;
  }
  return *result == symbol ? (char *)result : S21_NULL;
}
