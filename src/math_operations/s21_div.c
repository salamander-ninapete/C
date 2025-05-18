#include "s21_math_operations.h"

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int message = S21_OK;
  int code = 0;
  int scale_finish = 0;
  s21_decimal_set_zeroes(result);
  if (s21_is_equal(value_2, *result) == 1) {
    message = S21_NaN;
    return message;
  }
  int sign = s21_sign_check(value_1, value_2);
  if (sign == 1 || sign == 0)
    sign = S21_NEGATIVE;
  else
    sign = S21_POSITIVE;
  int scale_sourse =
      s21_scale_s21_decimal(value_1) - s21_scale_s21_decimal(value_2);
  char str[195];
  s21_get_result_in_string(value_1, value_2, str);
  s21_big_decimal result_big =
      s21_get_result_in_big_decimal(str, scale_sourse, &code, &scale_finish);
  if (code == 0) {
    s21_big_decimal_to_decimal(result_big, result);

    s21_decimal_set_scale(result, scale_finish);
    s21_decimal_set_sign(result, sign);
  } else {
    if (sign == S21_NEGATIVE)
      message = S21_MINUS_INF;
    else
      message = S21_PLUS_INF;
  }
  return message;
}
void s21_get_result_in_string(s21_decimal value_1, s21_decimal value_2,
                              char *str) {
  s21_decimal zero = {0};
  int l_str = 0;
  int first_index = 0;
  s21_decimal_set_sign(&value_1, 0);
  s21_decimal_set_sign(&value_2, 0);
  s21_decimal_set_scale(&value_1, 0);
  s21_decimal_set_scale(&value_2, 0);
  for (int i = MAX_BLOCK_NUMBER - 1; i >= 0; i--) {
    s21_decimal tmp1 = s21_decimal_right_shift(value_1, i);
    if (s21_is_greater_or_equal(tmp1, value_2)) {
      s21_decimal tmp2 = s21_left_shift_s21_decimal(value_2, i);
      s21_sub(value_1, tmp2, &value_1);
      str[l_str] = '1';
      l_str++;
      first_index = 1;
    } else {
      if (first_index == 1) {
        str[l_str] = '0';
        l_str++;
      }
    }
  }
  str[l_str] = '.';
  l_str++;
  if (s21_is_not_equal(value_1, zero) == 1) {
    for (int i = 0;
         i < MAX_BLOCK_NUMBER && s21_is_not_equal(value_1, zero) == 1; i++) {
      value_1 = s21_left_shift_s21_decimal(value_1, 1);
      if (s21_is_greater_or_equal(value_1, value_2) == 1) {
        s21_sub(value_1, value_2, &value_1);

        str[l_str] = '1';
        l_str++;

      } else {
        str[l_str] = '0';
        l_str++;
      }
    }
  }
  str[l_str] = '\0';
}
s21_big_decimal s21_get_result_in_big_decimal(char *str, int scale,
                                              int *message, int *scale_finish) {
  int scale_frac = 0;
  *scale_finish = 0;
  char *frac_str = calloc(100, sizeof(char));
  char *dot = strchr(str, '.');
  int index_dot = dot - str;
  s21_big_decimal integ2 = {0}, frac2 = {0}, res1 = {0};
  s21_decimal integ = {0}, frac = {0};
  strcpy(frac_str, str + index_dot + 1);
  frac_str[strlen(frac_str)] = '\0';
  long double tmp1 = s21_binary_to_double(frac_str, &scale_frac);
  s21_integer_to_binary(tmp1, frac_str);
  integ = s21_set_bit_string(integ, str, 0, index_dot);
  frac = s21_set_bit_string(frac, frac_str, 0, strlen(frac_str));

  s21_decimal_to_big_decimal(integ, &integ2);
  s21_decimal_to_big_decimal(frac, &frac2);

  *scale_finish = scale + scale_frac;
  while (scale_frac > 0) {
    s21_mul_on_ten_big_decimal(&integ2);
    scale_frac--;
  }
  s21_add_big_mantiss(integ2, frac2, &res1);
  while (*scale_finish < 0) {
    s21_mul_on_ten_big_decimal(&res1);
    *scale_finish += 1;
  }
  *message = s21_mul_round(res1, &res1, scale_finish);
  free(frac_str);
  return res1;
}
