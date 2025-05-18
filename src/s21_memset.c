#include "s21_string.h"

// Заполнение 1-ых n байтов в str символами symbols
void *s21_memset(void *str, int symbol, s21_size_t n) {
  if (str != S21_NULL) {
    unsigned char *ptr_str = (unsigned char *)str;
    for (s21_size_t i = 0; i < n; i++) {
      ptr_str[i] = (unsigned char)symbol;
    }
  }
  return str;
}