#include "s21_string.h"

// Разбивает строку на части, разделенный любыми символами из delim
char *s21_strtok(char *str, const char *delim) {
  static char *start_token = S21_NULL;

  char *end_token;
  char *result;

  if (str) start_token = str;
  while (start_token && *start_token) {
    while (*start_token && s21_strpbrk(start_token, delim) == start_token)
      start_token++;
    if (*start_token == '\0') break;
    end_token = s21_strpbrk(start_token, delim);

    if (end_token) {
      *end_token = '\0';
      result = start_token;
      start_token = end_token + 1;
    } else {
      result = start_token;
      start_token = S21_NULL;
    }
    if (*result != '\0') return result;
  }

  return S21_NULL;
}
