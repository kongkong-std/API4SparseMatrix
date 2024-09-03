#include "main.h"

void CooFileProcess(const char *path, Coo *mat)
{
    FILE *fp = NULL;
    if ((fp = fopen(path, "rb")) == NULL)
    {
        fprintf(stderr, "Cannot open file! - \'coo matrix file\'\n");
        exit(EXIT_FAILURE);
    }

    int n = 0, nnz = 0;
    fscanf(fp, "%d%d", &n, &nnz);
    mat->n = n;
    mat->nnz = nnz;
    if ((mat->row_idx = (int *)malloc(nnz * sizeof(int))) == NULL ||
        (mat->col_idx = (int *)malloc(nnz * sizeof(int))) == NULL ||
        (mat->val = (Scalar *)malloc(nnz * sizeof(Scalar))) == NULL)
    {
        fprintf(stderr, "Memory allocation failed! - \'coo matrix data\'\n");
        exit(EXIT_FAILURE);
    }

#ifdef ENABLE_COMPLEX
    for (int index = 0; index < nnz; ++index)
    {
        fscanf(fp, "%d%d%lf%lf", mat->row_idx + index,
               mat->col_idx + index,
               &(mat->val[index].re),
               &(mat->val[index].im));
    }
#else
    for (int index = 0; index < nnz; ++index)
    {
        fscanf(fp, "%d%d%lf", mat->row_idx + index,
               mat->col_idx + index,
               mat->val + index);
    }
#endif

    fclose(fp);
}

void CsrFileProcess(const char *path, const Csr *mat)
{
    FILE *fp = NULL;
    if ((fp = fopen(path, "wb")) == NULL)
    {
        fprintf(stderr, "Cannot open file! - \'csr matrix data\'\n");
        exit(EXIT_FAILURE);
    }

    int n = mat->n, nnz = mat->nnz;
    fprintf(fp, "%d\n%d\n", n, nnz);
    for (int index = 0; index < n; ++index)
    {
        fprintf(fp, "%d\n", mat->row_idx[index]);
    }
    for (int index = 0; index < n + 1; ++index)
    {
        fprintf(fp, "%d\n", mat->row_ptr[index]);
    }
    for (int index = 0; index < nnz; ++index)
    {
        fprintf(fp, "%d\n", mat->col_idx[index]);
    }
    for (int index = 0; index < nnz; ++index)
    {
#ifdef ENABLE_COMPLEX
        fprintf(fp, "%021.16le\t%021.16le\n", mat->val[index].re, mat->val[index].im);
#else
        fprintf(fp, "%021.16le\n", mat->val[index]);
#endif
    }

    fclose(fp);
}