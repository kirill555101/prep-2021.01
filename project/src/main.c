#include <stdio.h>
#include "matrix.h"

#define CREATE_MATRIX_FROM_FILE_ERROR 1
#define CREATE_MATRIX_ERROR 2
#define GET_ROWS_ERROR 3
#define GET_COLS_ERROR 4
#define GET_ELEM_ERROR 5
#define SET_ELEM_ERROR 6
#define MUL_SCALAR_ERROR 7
#define TRANSP_ERROR 8
#define SUM_ERROR 9
#define SUB_ERROR 10
#define MUL_ERROR 11
#define DET_ERROR 12
#define ADJ_ERROR 13
#define INV_ERROR 14

#define TEST_FILENAME "test.txt"

int main() {
    Matrix* first_matrix = create_matrix_from_file(TEST_FILENAME);
    if (first_matrix == NULL) {
        puts("FAILURE create_matrix_from_file");
        return CREATE_MATRIX_FROM_FILE_ERROR;
    }
    puts("SUCCESS create_matrix_from_file");

    Matrix* second_matrix = create_matrix(4, 4);
    if (second_matrix == NULL) {
        puts("FAILURE create_matrix");
        return CREATE_MATRIX_ERROR;
    }
    puts("SUCCESS create_matrix");

    size_t rows, cols;
    if (get_rows(first_matrix, &rows) != 0) {
        puts("FAILURE get_rows");
        return GET_ROWS_ERROR;
    }
    puts("SUCCESS get_rows");

    if (get_rows(first_matrix, &cols) != 0) {
        puts("FAILURE get_cols");
        return GET_COLS_ERROR;
    }
    puts("SUCCESS get_cols");

    double val;
    if (get_elem(second_matrix, 2, 3, &val) != 0) {
        puts("FAILURE get_elem");
        return GET_ELEM_ERROR;
    }
    puts("SUCCESS get_elem");

    if (set_elem(second_matrix, 1, 4, val) != 0) {
        puts("FAILURE set_elem");
        return SET_ELEM_ERROR;
    }
    puts("SUCCESS set_elem");

    Matrix* muled_on_scalar_matrix = mul_scalar(second_matrix, 5);
    if (muled_on_scalar_matrix == NULL) {
        puts("FAILURE mul_scalar");
        return MUL_SCALAR_ERROR;
    }
    puts("SUCCESS mul_scalar");

    Matrix* transposed_matrix = transp(second_matrix);
    if (transposed_matrix != NULL) {
        puts("FAILURE transp");
        return TRANSP_ERROR;
    }
    puts("SUCCESS transp");

    Matrix* summed_matrix = sum(first_matrix, second_matrix);
    if (summed_matrix == NULL) {
        puts("FAILURE sum");
        return SUM_ERROR;
    }
    puts("SUCCESS sum");

    Matrix* subbed_matrix = sub(first_matrix, second_matrix);
    if (subbed_matrix == NULL) {
        puts("FAILURE sub");
        return SUB_ERROR;
    }
    puts("SUCCESS sub");

    Matrix* mulled_matrix = mul(first_matrix, second_matrix);
    if (mulled_matrix == NULL) {
        puts("FAILURE mul");
        return MUL_ERROR;
    }
    puts("SUCCESS mul");

    if (det(first_matrix, &val) != 0) {
        puts("FAILURE det");
        return DET_ERROR;
    }
    puts("SUCCESS det");
    printf("%f\n", val);

    Matrix* third_matrix = create_matrix_from_file(TEST_FILENAME);

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            printf("%10.4f\t", third_matrix->data[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    Matrix* adjointed_matrix = adj(third_matrix);
    if (adjointed_matrix == NULL) {
        puts("FAILURE adj");
        return ADJ_ERROR;
    }
    puts("SUCCESS adj");

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            printf("%10.4f\t", adjointed_matrix->data[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    Matrix* inverted_matrix = inv(third_matrix);
    if (inverted_matrix == NULL) {
        puts("FAILURE inv");
        return INV_ERROR;
    }
    puts("SUCCESS inv");

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            printf("%10.4f\t", inverted_matrix->data[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    free_matrix(first_matrix);
    free_matrix(second_matrix);
    free_matrix(third_matrix);
    free_matrix(muled_on_scalar_matrix);
    free_matrix(transposed_matrix);
    free_matrix(summed_matrix);
    free_matrix(subbed_matrix);
    free_matrix(mulled_matrix);
    free_matrix(adjointed_matrix);
    free_matrix(inverted_matrix);

    return 0;
}
