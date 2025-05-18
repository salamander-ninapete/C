#include "s21_string.h"

// Сравнение 2-х блоков памяти в первых n байтах
int s21_memcmp(const void *str1, const void *str2, s21_size_t n) {
  int result = 0;

  const unsigned char *ptr_str1 = (const unsigned char *)str1;
  const unsigned char *ptr_str2 = (const unsigned char *)str2;
  for (s21_size_t i = 0; i < n && result == 0; i++) {
    if (ptr_str1[i] != ptr_str2[i]) {
      result = (int)ptr_str1[i] - (int)ptr_str2[i];
    }
  }
  if (str1 != str2) {
    if (str1 == S21_NULL) {
      result = -1;
    }
    if (str2 == S21_NULL) {
      result = 1;
    }
  }
  return result;
}
