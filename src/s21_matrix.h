#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define SUCCESS 1
#define FAILURE 0

#define N_CALC 2
#define N_CORR 1
#define OK 0

typedef struct matrix_struct {
  double** matrix;
  int rows;
  int columns;
} matrix_t;

// enum returnim { OK, N_СORR, N_CALC };

int s21_create_matrix(int rows, int columns, matrix_t* result);
void s21_remove_matrix(matrix_t* A);
int s21_eq_matrix(matrix_t* A, matrix_t* B);
int s21_sum_matrix(matrix_t* A, matrix_t* B, matrix_t* result);
int s21_sub_matrix(matrix_t* A, matrix_t* B, matrix_t* result);
int s21_mult_number(matrix_t* A, double number, matrix_t* result);
int s21_mult_matrix(matrix_t* A, matrix_t* B, matrix_t* result);
int s21_transpose(matrix_t* A, matrix_t* result);
int s21_determinant(matrix_t* A, double* result);
int s21_calc_complements(matrix_t* A, matrix_t* result);
int s21_inverse_matrix(matrix_t* A, matrix_t* result);

int s21_norm_znach(int rows, int columns);
int s21_norm_for_math(matrix_t* A, matrix_t* B, int x);
void s21_print_matrix(matrix_t* result);
int s21_generate_num_for_matrix(matrix_t* result, int err);
double s21_mul_two_mat(matrix_t* A, matrix_t* B, int r, int c);
void s21_obrezka(matrix_t* A, matrix_t* result, int ignorR, int ignorC);
double s21_dtr_for_2(matrix_t* A);
double s21_dtr_for_3(matrix_t* A);
double s21_ignor_det(matrix_t* A, int R, int C);
