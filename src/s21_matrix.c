#include "s21_matrix.h"

int s21_norm_znach(int rows, int columns) {
  int answer = SUCCESS;
  if (rows <= 0 || columns <= 0) {
    answer = FAILURE;
  }
  return answer;
}

int s21_norm_for_math(matrix_t* A, matrix_t* B, int x) {
  int answer = OK;
  if (x == 1) {
    if (A == NULL || B == NULL) {
      answer = N_CORR;
    } else if (!s21_norm_znach(A->rows, A->columns) ||
               !s21_norm_znach(B->rows, B->columns)) {
      answer = N_CORR;
    } else if ((A->columns != B->columns) || (A->rows != B->rows)) {
      answer = N_CALC;
    }
  } else if (x == 2) {
    if (A == NULL || B == NULL) {
      answer = N_CORR;
    } else if (!s21_norm_znach(A->rows, A->columns)) {
      answer = N_CORR;
    }
  } else if (x == 3) {
    if (A == NULL || B == NULL) {
      answer = N_CORR;
    } else if (!s21_norm_znach(A->rows, A->columns) ||
               !s21_norm_znach(B->rows, B->columns)) {
      answer = N_CORR;
    } else if (A->columns != B->rows) {
      answer = N_CALC;
    }
  } else if (x == 4) {
    if (A == NULL || B == NULL) {
      answer = N_CORR;
    } else if (!s21_norm_znach(A->rows, A->columns)) {
      answer = N_CORR;
    } else if (A->columns != A->rows) {
      answer = N_CALC;
    }
  }
  return answer;
}

int s21_create_matrix(int rows, int columns, matrix_t* result) {
  int answer = 0;

  if (result == NULL || !s21_norm_znach(rows, columns)) {
    answer = N_CORR;
    return answer;
  }
  result->rows = rows;
  result->columns = columns;
  result->matrix = (double**)malloc(sizeof(double*) * rows);
  if (result->matrix) {
    for (int i = 0; i < rows; i++) {
      result->matrix[i] = (double*)malloc(sizeof(double) * columns);
      if (!result->matrix[i]) {
        answer = N_CORR;
        break;
      }
    }
    if (!answer) {
      for (int r = 0; r < rows; r++) {
        for (int c = 0; c < columns; c++) {
          result->matrix[r][c] = 0;
        }
      }
    }
  } else {
    answer = N_CORR;
  }
  return answer;
}

void s21_remove_matrix(matrix_t* A) {
  if (A != NULL && A->matrix != NULL && (A->rows != 0 && A->columns != 0)) {
    for (int r = 0; r < A->rows; r++) {
      free(A->matrix[r]);
    }
    free(A->matrix);
    A->matrix = NULL;
    A->rows = 0;
    A->columns = 0;
  }
}

int s21_eq_matrix(matrix_t* A, matrix_t* B) {
  int answer = SUCCESS;
  if (A == NULL || B == NULL) {
    answer = FAILURE;
  } else if (!s21_norm_znach(A->rows, A->columns) ||
             !s21_norm_znach(B->rows, B->columns)) {
    answer = FAILURE;
  } else if ((A->columns != B->columns) || (A->rows != B->rows)) {
    answer = FAILURE;
  } else {
    for (int r = 0; r < A->rows && answer != 0; r++) {
      for (int c = 0; c < A->columns && answer != 0; c++) {
        double a = fabs(A->matrix[r][c]);
        double b = fabs(B->matrix[r][c]);
        double D = a - b;
        D = fabs(D);
        if (D >= 0.0000001) answer = FAILURE;
      }
    }
  }
  return answer;
}

int s21_sum_matrix(matrix_t* A, matrix_t* B, matrix_t* result) {
  int answer = s21_norm_for_math(A, B, 1);
  if (!answer) {
    s21_create_matrix(A->rows, A->columns, result);
    for (int r = 0; r < result->rows; r++) {
      for (int c = 0; c < result->columns; c++) {
        result->matrix[r][c] = A->matrix[r][c] + B->matrix[r][c];
      }
    }
  }
  return answer;
}

int s21_sub_matrix(matrix_t* A, matrix_t* B, matrix_t* result) {
  int answer = s21_norm_for_math(A, B, 1);
  if (!answer) {
    s21_create_matrix(A->rows, A->columns, result);
    for (int r = 0; r < result->rows; r++) {
      for (int c = 0; c < result->columns; c++) {
        result->matrix[r][c] = A->matrix[r][c] - B->matrix[r][c];
      }
    }
  }
  return answer;
}

int s21_mult_number(matrix_t* A, double number, matrix_t* result) {
  int answer = s21_norm_for_math(A, result, 2);
  if (!answer) {
    s21_create_matrix(A->rows, A->columns, result);
    for (int r = 0; r < result->rows; r++) {
      for (int c = 0; c < result->columns; c++) {
        if (A->rows != 1 && A->columns != 1) {
          result->matrix[r][c] = A->matrix[r][c] * number;
        } else {
          result->matrix[r][c] = 1 / A->matrix[r][c];
        }
      }
    }
  }
  return answer;
}

double s21_mul_two_mat(matrix_t* A, matrix_t* B, int r, int c) {
  double C = 0;
  for (int c1 = 0, r2 = 0; r2 < B->rows; c1++, r2++) {
    C += A->matrix[r][c1] * B->matrix[r2][c];
  }
  return C;
}

int s21_mult_matrix(matrix_t* A, matrix_t* B, matrix_t* result) {
  int answer = s21_norm_for_math(A, B, 3);
  if (!answer) {
    s21_create_matrix(A->rows, B->columns, result);
    for (int r = 0; r < result->rows; r++) {
      for (int c = 0; c < result->columns; c++) {
        result->matrix[r][c] = s21_mul_two_mat(A, B, r, c);
      }
    }
  }
  return answer;
}

int s21_transpose(matrix_t* A, matrix_t* result) {
  int answer = s21_norm_for_math(A, result, 2);
  if (!answer) {
    s21_create_matrix(A->columns, A->rows, result);
    for (int r = 0, cc = 0; r < result->rows; r++, cc++) {
      for (int c = 0, rr = 0; c < result->columns; c++, rr++) {
        result->matrix[r][c] = A->matrix[rr][cc];
      }
    }
  }
  return answer;
}

void s21_obrezka(matrix_t* A, matrix_t* result, int ignorR, int ignorC) {
  s21_create_matrix(A->rows - 1, A->columns - 1, result);
  int rr = 0, cc = 0;
  for (int r = 0; r < A->rows; r++) {
    if (r == ignorR) continue;
    cc = 0;
    for (int c = 0; c < A->columns; c++) {
      if (c == ignorC) continue;
      result->matrix[rr][cc] = A->matrix[r][c];
      cc++;
    }
    rr++;
  }
}

double s21_dtr_for_2(matrix_t* A) {
  return ((A->matrix[0][0] * A->matrix[1][1]) -
          (A->matrix[1][0] * A->matrix[0][1]));
}

double s21_dtr_for_3(matrix_t* A) {
  double res = 0, d1 = 0, d2 = 0, d3 = 0;
  d1 = A->matrix[0][0] *
       (A->matrix[1][1] * A->matrix[2][2] - A->matrix[1][2] * A->matrix[2][1]);
  d2 = -A->matrix[0][1] *
       (A->matrix[1][0] * A->matrix[2][2] - A->matrix[1][2] * A->matrix[2][0]);
  d3 = A->matrix[0][2] *
       (A->matrix[1][0] * A->matrix[2][1] - A->matrix[1][1] * A->matrix[2][0]);
  res = d1 + d2 + d3;
  return res;
}

int s21_determinant(matrix_t* A, double* result) {
  int answer = s21_norm_for_math(A, A, 4);
  if (!answer) {
    if (result != NULL) {
      if (A->rows > 3) {
        double det = 0;
        for (int i = 0; i < A->columns; i++) {
          matrix_t a;
          s21_obrezka(A, &a, 0, i);
          s21_determinant(&a, &det);
          *result += A->matrix[0][i] * det * pow(-1, i);
          s21_remove_matrix(&a);
        }
      }
      if (A->rows == 3) {
        *result = s21_dtr_for_3(A);
      }
      if (A->rows == 2) {
        *result = s21_dtr_for_2(A);
      }
      if (A->rows == 1) {
        *result = A->matrix[0][0];
      }
    } else {
      answer = N_CORR;
    }
  }
  return answer;
}

double s21_ignor_det(matrix_t* A, int R, int C) {
  matrix_t a;
  double det = 0;
  if (A->rows > 1) {
    s21_obrezka(A, &a, R, C);
  } else {
    a = *A;
  }
  s21_determinant(&a, &det);
  s21_remove_matrix(&a);
  return det;
}

int s21_calc_complements(matrix_t* A, matrix_t* result) {
  int answer = s21_norm_for_math(A, result, 4);
  if (!answer) {
    s21_create_matrix(A->rows, A->columns, result);
    if (A->rows == 1 && A->columns == 1) {
      result->matrix[0][0] = A->matrix[0][0];
    } else {
      for (int r = 0; r < result->rows; r++) {
        for (int c = 0; c < result->columns; c++) {
          result->matrix[r][c] = s21_ignor_det(A, r, c) * pow(-1, c + r);
        }
      }
    }
  }
  return answer;
}

int s21_inverse_matrix(matrix_t* A, matrix_t* result) {
  int answer = s21_norm_for_math(A, result, 4);
  if (!answer) {
    matrix_t a, aa;
    double det = 0;
    s21_determinant(A, &det);
    if (det != 0) {
      if (A->rows != 1) {
        s21_calc_complements(A, &a);
        s21_transpose(&a, &aa);
        s21_remove_matrix(&a);
        s21_mult_number(&aa, (double)1 / det, result);
        s21_remove_matrix(&aa);
      } else {
        s21_mult_number(A, det, result);
      }
    } else {
      answer = N_CALC;
    }
  }
  return answer;
}

// void s21_print_matrix(matrix_t* result) {
//   if (s21_norm_znach(result->rows, result->columns) && result != NULL) {
//     for (int r = 0; r < result->rows; r++) {
//       for (int c = 0; c < result->columns; c++) {
//         printf(" %lf |", result->matrix[r][c]);
//       }
//       printf("\n");
//     }
//     printf("\n");

//   } else {
//     printf("ne to\n");
//   }
// }

// int s21_generate_num_for_matrix(matrix_t* result, int err) {
//   int answer = 0;
//   if (s21_norm_znach(result->rows, result->columns) && !err) {
//     double num = 0;
//     for (int r = 0; r < result->rows; r++) {
//       for (int c = 0; c < result->columns; c++) {
//         result->matrix[r][c] = num;
//         num = num + 1.0000000000;
//       }
//     }

//     // result->matrix[0][0] = 2;
//     // result->matrix[0][1] = 5;
//     // result->matrix[0][2] = 7;
//     // result->matrix[0][3] = 3;
//     // result->matrix[0][4] = -11;

//     // result->matrix[1][0] = 6;
//     // result->matrix[1][1] = 3;
//     // result->matrix[1][2] = 4;
//     // result->matrix[1][3] = 7;
//     // result->matrix[1][4] = 1;

//     // result->matrix[2][0] = 5;
//     // result->matrix[2][1] = -2;
//     // result->matrix[2][2] = -3;
//     // result->matrix[2][3] = -2;
//     // result->matrix[2][4] = -6;

//     // result->matrix[3][0] = 4;
//     // result->matrix[3][1] = 1;
//     // result->matrix[3][2] = -5;
//     // result->matrix[3][3] = 6;
//     // result->matrix[3][4] = 9;

//     // result->matrix[4][0] = 7;
//     // result->matrix[4][1] = 1;
//     // result->matrix[4][2] = 9;
//     // result->matrix[4][3] = 6;
//     // result->matrix[4][4] = 9;
//   } else {
//     answer = N_CORR;
//   }
//   return answer;
// }