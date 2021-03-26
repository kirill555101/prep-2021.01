#include <stdio.h>
#include "matrix.h"

typedef enum {
    create_matrix_from_file_error = 1,
    create_matrix_error,
    get_rows_error,
    get_cols_error,
    get_elem_error,
    set_elem_error,
    mul_scalar_error,
    transp_error,
    sum_error,
    sub_error,
    mul_error,
    det_error,
    adj_error,
    inv_error
} ERROR;

#define TEST_FILENAME "test.txt"

int main() {
    Matrix* first_matrix = create_matrix_from_file(TEST_FILENAME);
    if (first_matrix == NULL) {
        fprintf(stderr, "FAILURE create_matrix_from_file");
        return create_matrix_from_file_error;
    }
    fprintf(stdin, "SUCCESS create_matrix_from_file");

    Matrix* second_matrix = create_matrix(4, 4);
    if (second_matrix == NULL) {
        fprintf(stderr, "FAILURE create_matrix");
        free_matrix(first_matrix);
        return create_matrix_error;
    }
    fprintf(stdin, "SUCCESS create_matrix");

    size_t rows, cols;
    if (get_rows(first_matrix, &rows) != 0) {
        fprintf(stderr, "FAILURE get_rows");
        free_matrix(first_matrix);
        free_matrix(second_matrix);
        return get_rows_error;
    }
    fprintf(stdin, "SUCCESS get_rows");

    if (get_cols(first_matrix, &cols) != 0) {
        fprintf(stderr, "FAILURE get_cols");
        free_matrix(first_matrix);
        free_matrix(second_matrix);
        return get_cols_error;
    }
    fprintf(stdin, "SUCCESS get_cols");

    double val;
    if (get_elem(second_matrix, 2, 3, &val) != 0) {
        fprintf(stderr, "FAILURE get_elem");
        free_matrix(first_matrix);
        free_matrix(second_matrix);
        return get_elem_error;
    }
    fprintf(stdin, "SUCCESS get_elem");

    if (set_elem(second_matrix, 1, 4, val) != 0) {
        fprintf(stderr, "FAILURE set_elem");
        free_matrix(first_matrix);
        free_matrix(second_matrix);
        return set_elem_error;
    }
    fprintf(stdin, "SUCCESS set_elem");

    Matrix* muled_on_scalar_matrix = mul_scalar(second_matrix, 5);
    if (muled_on_scalar_matrix == NULL) {
        fprintf(stderr, "FAILURE mul_scalar");
        free_matrix(first_matrix);
        free_matrix(second_matrix);
        return mul_scalar_error;
    }
    fprintf(stdin, "SUCCESS mul_scalar");
    free_matrix(muled_on_scalar_matrix);

    Matrix* transposed_matrix = transp(second_matrix);
    if (transposed_matrix != NULL) {
        fprintf(stderr, "FAILURE transp");
        free_matrix(first_matrix);
        free_matrix(second_matrix);
        return transp_error;
    }
    fprintf(stdin, "SUCCESS transp");
    free_matrix(transposed_matrix);

    Matrix* summed_matrix = sum(first_matrix, second_matrix);
    if (summed_matrix == NULL) {
        fprintf(stderr, "FAILURE sum");
        free_matrix(first_matrix);
        free_matrix(second_matrix);
        return sum_error;
    }
    fprintf(stdin, "SUCCESS sum");
    free_matrix(summed_matrix);

    Matrix* subbed_matrix = sub(first_matrix, second_matrix);
    if (subbed_matrix == NULL) {
        fprintf(stderr, "FAILURE sub");
        free_matrix(first_matrix);
        free_matrix(second_matrix);
        return sub_error;
    }
    fprintf(stdin, "SUCCESS sub");
    free_matrix(subbed_matrix);

    Matrix* mulled_matrix = mul(first_matrix, second_matrix);
    if (mulled_matrix == NULL) {
        fprintf(stderr, "FAILURE mul");
        free_matrix(first_matrix);
        free_matrix(second_matrix);
        return mul_error;
    }
    fprintf(stdin, "SUCCESS mul");
    free_matrix(mulled_matrix);

    if (det(first_matrix, &val) != 0) {
        fprintf(stderr, "FAILURE det");
        free_matrix(first_matrix);
        free_matrix(second_matrix);
        return det_error;
    }
    fprintf(stdin, "SUCCESS det");

    Matrix* third_matrix = create_matrix_from_file(TEST_FILENAME);
    if (third_matrix == NULL) {
        fprintf(stderr, "FAILURE create_matrix_from_file");
        free_matrix(first_matrix);
        free_matrix(second_matrix);
        return create_matrix_from_file_error;
    }
    fprintf(stdin, "SUCCESS create_matrix_from_file");

    Matrix* adjointed_matrix = adj(third_matrix);
    if (adjointed_matrix == NULL) {
        fprintf(stderr, "FAILURE adj");
        free_matrix(first_matrix);
        free_matrix(second_matrix);
        free_matrix(third_matrix);
        return adj_error;
    }
    fprintf(stdin, "SUCCESS adj");
    free_matrix(adjointed_matrix);
    Matrix* inverted_matrix = inv(third_matrix);
    if (inverted_matrix == NULL) {
        fprintf(stderr, "FAILURE inv");
        free_matrix(first_matrix);
        free_matrix(second_matrix);
        free_matrix(third_matrix);
        return inv_error;
    }
    fprintf(stdin, "SUCCESS inv");

    free_matrix(first_matrix);
    free_matrix(second_matrix);
    free_matrix(third_matrix);
    free_matrix(inverted_matrix);

    return 0;
}
