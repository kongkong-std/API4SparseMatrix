#include "main.h"

void MatrixFree(Matrix *mat)
{
    free(mat->row_idx);
    free(mat->row_ptr);
    free(mat->col_idx);
    free(mat->val);
}

void VectorFree(Vector *vec)
{
    free(vec->row_idx);
    free(vec->val);
}

void MatrixByVectorProduct(const Matrix *mat, const Vector *vec_x,
                           Vector *vec)
{
    int n = mat->n;
    vec->n = n;

    if ((vec->row_idx = (int *)malloc(n * sizeof(int))) == NULL ||
        (vec->val = (Scalar *)malloc(n * sizeof(Scalar))) == NULL)
    {
        fprintf(stderr, "Memory allocation failed! - \'result vector data\'\n");
        exit(EXIT_FAILURE);
    }

    for (int index = 0; index < n; ++index)
    {
        vec->row_idx[index] = vec_x->row_idx[index];
        int index_start = mat->row_ptr[index];
        int index_end = mat->row_ptr[index + 1];
#ifdef ENABLE_COMPLEX
        double tmp_re = 0., tmp_im = 0.;
#else
        double tmp = 0.;
#endif
        for (int index_j = index_start; index_j < index_end; ++index_j)
        {
#ifdef ENABLE_COMPLEX
            tmp_re += mat->val[index_j].re * vec_x->val[mat->col_idx[index_j]].re -
                      mat->val[index_j].im * vec_x->val[mat->col_idx[index_j]].im;
            tmp_im += mat->val[index_j].re * vec_x->val[mat->col_idx[index_j]].im +
                      mat->val[index_j].im * vec_x->val[mat->col_idx[index_j]].re;
#else
            tmp += mat->val[index_j] * vec_x->val[mat->col_idx[index_j]];
#endif
        }
#ifdef ENABLE_COMPLEX
        vec->val[index].re = tmp_re;
        vec->val[index].im = tmp_im;
#else
        mat->val[index] = tmp;
#endif
    }
}
