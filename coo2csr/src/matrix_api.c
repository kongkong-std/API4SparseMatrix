#include "main.h"

void Coo2Csr(const Coo *mat_coo, Csr *mat_csr)
{
    int n = 0, nnz = 0;
    n = mat_coo->n;
    nnz = mat_coo->nnz;

    if ((mat_csr->row_idx = (int *)malloc(n * sizeof(int))) == NULL ||
        (mat_csr->row_ptr = (int *)malloc((n + 1) * sizeof(int))) == NULL ||
        (mat_csr->col_idx = (int *)malloc(nnz * sizeof(int))) == NULL ||
        (mat_csr->val = (Scalar *)malloc(nnz * sizeof(Scalar))) == NULL)
    {
        fprintf(stderr, "Memory allocation failed! - \'csr matrix data\'\n");
        exit(EXIT_FAILURE);
    }
    mat_csr->n = n;
    mat_csr->nnz = nnz;

#if 0
    for (int index_i = 0; index_i < nnz; ++index_i)
    {
        for (int index_j = index_i + 1; index_j < nnz; ++index_j)
        {
            int tmp_idx;
            Scalar tmp_val;
            if (mat_coo->row_idx[index_j] < mat_coo->row_idx[index_i])
            {
                // swap row_idx[index_i] <-> row_idx[index_j]
                tmp_idx = mat_coo->row_idx[index_j];
                mat_coo->row_idx[index_j] = mat_coo->row_idx[index_i];
                mat_coo->row_idx[index_i] = tmp_idx;

                // swap col_idx[index_i] <-> col_idx[index_j]
                tmp_idx = mat_coo->col_idx[index_j];
                mat_coo->col_idx[index_j] = mat_coo->col_idx[index_i];
                mat_coo->col_idx[index_i] = tmp_idx;

                // swap val[index_i] <-> val[index_j]
                tmp_val = mat_coo->val[index_j];
                mat_coo->val[index_j] = mat_coo->val[index_i];
                mat_coo->val[index_i] = tmp_val;
            }
        }
    }
#endif

    for (int index = 0; index < n; ++index)
    {
        mat_csr->row_idx[index] = index;
    }

    int cnt_row = 0;
    mat_csr->row_ptr[0] = 0;
    for (int index = 0; index < n; ++index)
    {
        for (int index_i = 0; index_i < nnz; ++index_i)
        {
            if (mat_coo->row_idx[index_i] == index)
            {
                mat_csr->col_idx[cnt_row] = mat_coo->col_idx[index_i];
                mat_csr->val[cnt_row] = mat_coo->val[index_i];
                ++cnt_row;
            }
        }
        mat_csr->row_idx[index + 1] = cnt_row;
    }
}

void CooMatrixFree(Coo *mat)
{
    free(mat->row_idx);
    free(mat->col_idx);
    free(mat->val);
}

void CsrMatrixFree(Csr *mat)
{
    free(mat->row_idx);
    free(mat->row_ptr);
    free(mat->col_idx);
    free(mat->val);
}
