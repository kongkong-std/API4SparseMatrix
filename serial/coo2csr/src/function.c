#include "main.h"

#define MAX_SIZE 1024

#ifdef TYPE_REAL
typedef struct
{
    /* data */
    int row_idx, col_idx;
    double val;
} COOElement;
#elif defined(TYPE_COMPLEX)
typedef struct
{
    /* data */
    int row_idx, col_idx;
    double val_re, val_im;
} COOElement;
#endif

int CompareElement(const void *a, const void *b)
{
    COOElement *elem1 = (COOElement *)a;
    COOElement *elem2 = (COOElement *)b;

    if (elem1->row_idx != elem2->row_idx)
    {
        return elem1->row_idx - elem2->row_idx;
    }
    else
    {
        return elem1->col_idx - elem2->col_idx;
    }
}

void COOMatrixSortAscend(MatCOO *mat)
{
    int nnz = mat->nnz;
    COOElement *element = NULL;
    if ((element = (COOElement *)malloc(nnz * sizeof(COOElement))) == NULL)
    {
        fprintf(stderr, "Memory allocation failed! - coo sort ascending order\n");
        exit(EXIT_FAILURE);
    }

    for (int index = 0; index < nnz; ++index)
    {
        element[index].row_idx = mat->row_idx[index];
        element[index].col_idx = mat->col_idx[index];
#ifdef TYPE_REAL
        element[index].val = mat->val[index];
#elif defined(TYPE_COMPLEX)
        element[index].val_re = mat->val_re[index];
        element[index].val_im = mat->val_im[index];
#endif
    }

    qsort(element, nnz, sizeof(COOElement), CompareElement);

    for (int index = 0; index < nnz; ++index)
    {
        mat->row_idx[index] = element[index].row_idx;
        mat->col_idx[index] = element[index].col_idx;
#ifdef TYPE_REAL
        mat->val[index] = element[index].val;
#elif defined(TYPE_COMPLEX)
        mat->val_re[index] = element[index].val_re;
        mat->val_im[index] = element[index].val_im;
#endif
    }

    // free memory
    free(element);
}

void COOMatrix2CSRMatrix(const MatCOO *mat_coo, int base_coo,
                         MatCSR *mat_csr, int base_csr)
{
    int n = 0, nnz = 0;
    n = mat_coo->n;
    nnz = mat_coo->nnz;
    mat_csr->n = n;
    mat_csr->nnz = nnz;

    if ((mat_csr->row_idx = (int *)malloc(n * sizeof(int))) == NULL ||
        (mat_csr->row_ptr = (int *)malloc((n + 1) * sizeof(int))) == NULL ||
        (mat_csr->col_idx = (int *)malloc(nnz * sizeof(int))) == NULL)
    {
        fprintf(stderr, "Memory allocation failed! - csr row and column data\n");
        exit(EXIT_FAILURE);
    }

    for (int index = 0; index < n; ++index)
    {
        mat_csr->row_idx[index] = index + base_csr;
    }

    // nnz in each row
    int *nnz_each_row = NULL;
    if ((nnz_each_row = (int *)malloc(n * sizeof(int))) == NULL)
    {
        fprintf(stderr, "Memory allocation failed! - csr each row nnz\n");
        exit(EXIT_FAILURE);
    }
    memset(nnz_each_row, 0, n * sizeof(int));

    for (int index = 0; index < nnz; ++index)
    {
        ++nnz_each_row[mat_coo->row_idx[index] - base_coo];
    }

    // row_ptr
    mat_csr->row_ptr[0] = base_csr;
    for (int index = 0; index < n; ++index)
    {
        mat_csr->row_ptr[index + 1] = mat_csr->row_ptr[index] + nnz_each_row[index];
    }

    int *tmp_nnz_each_row = NULL;
    if ((tmp_nnz_each_row = (int *)malloc(n * sizeof(int))) == NULL)
    {
        fprintf(stderr, "Memory allocation failed! - temporary nnz in each row\n");
        exit(EXIT_FAILURE);
    }
    memset(tmp_nnz_each_row, 0, n * sizeof(int));

#ifdef TYPE_REAL
    if ((mat_csr->val = (double *)malloc(nnz * sizeof(double))) == NULL)
    {
        fprintf(stderr, "Memory allocation failed! - csr value\n");
        exit(EXIT_FAILURE);
    }

    for (int index = 0; index < nnz; ++index)
    {
        int row_tmp = mat_coo->row_idx[index] - base_coo;
        int dst_tmp = mat_csr->row_ptr[row_tmp] + tmp_nnz_each_row[row_tmp];
        mat_csr->col_idx[dst_tmp - base_csr] = mat_coo->col_idx[index] + (base_csr - base_coo);
        mat_csr->val[dst_tmp - base_csr] = mat_coo->val[index];
        ++tmp_nnz_each_row[row_tmp];
    }
#elif defined(TYPE_COMPLEX)
    if ((mat_csr->val_re = (double *)malloc(nnz * sizeof(double))) == NULL ||
        (mat_csr->val_im = (double *)malloc(nnz * sizeof(double))) == NULL)
    {
        fprintf(stderr, "Memory allocation failed! - csr value\n");
        exit(EXIT_FAILURE);
    }

    for (int index = 0; index < nnz; ++index)
    {
        int row_tmp = mat_coo->row_idx[index] - base_coo;
        int dst_tmp = mat_csr->row_ptr[row_tmp] + tmp_nnz_each_row[row_tmp];
        mat_csr->col_idx[dst_tmp - base_csr] = mat_coo->col_idx[index] + (base_csr - base_coo);
        mat_csr->val_re[dst_tmp - base_csr] = mat_coo->val_re[index];
        mat_csr->val_im[dst_tmp - base_csr] = mat_coo->val_im[index];
        ++tmp_nnz_each_row[row_tmp];
    }
#endif

    // free memory
    free(tmp_nnz_each_row);
    free(nnz_each_row);
}

void CSRMatrixFileOutput(const char *path, const MatCSR *mat)
{
    FILE *fp = NULL;
    if ((fp = fopen(path, "wb")) == NULL)
    {
        fprintf(stderr, "Cannot open file - \'%s\'\n", path);
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
#ifdef TYPE_REAL
        fprintf(fp, "%021.16le\n", mat->val[index]);
#elif defined(TYPE_COMPLEX)
        fprintf(fp, "%021.16le\t%021.16le\n", mat->val_re[index], mat->val_im[index]);
#endif
    }

    fclose(fp);
}

void COOMatrixFileOutput(const char *path, const MatCOO *mat)
{
    FILE *fp = NULL;
    if ((fp = fopen(path, "wb")) == NULL)
    {
        fprintf(stderr, "Cannot open file - \'%s\'\n", path);
        exit(EXIT_FAILURE);
    }

    int m = mat->m, n = mat->n, nnz = mat->nnz;
    fprintf(fp, "%d\t%d\t%d\n", m, n, nnz);
    for (int index = 0; index < nnz; ++index)
    {
#ifdef TYPE_REAL
        fprintf(fp, "%d\t%d\t%021.16le\n", mat->row_idx[index], mat->col_idx[index], mat->val[index]);
#elif defined(TYPE_COMPLEX)
        fprintf(fp, "%d\t%d\t%021.16le\t%021.16le\n", mat->row_idx[index],
                mat->col_idx[index], mat->val_re[index], mat->val_im[index]);
#endif
    }

    fclose(fp);
}

void CSRMatrixDataFree(MatCSR *mat)
{
    free(mat->row_idx);
    free(mat->row_ptr);
    free(mat->col_idx);
#ifdef TYPE_REAL
    free(mat->val);
#elif defined(TYPE_COMPLEX)
    free(mat->val_re);
    free(mat->val_im);
#endif
}

void COOMatrixFileProcess(const char *path, MatCOO *mat)
{
    FILE *fp = NULL;
    if ((fp = fopen(path, "rb")) == NULL)
    {
        fprintf(stderr, "Cannot open file - \'%s\'\n", path);
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_SIZE];
    while (fgets(buffer, MAX_SIZE, fp) != NULL)
    {
        if (buffer[0] != '%')
        {
            break;
        }
    }

    int m = 0, n = 0, nnz = 0;

    sscanf(buffer, "%d%d%d", &m, &n, &nnz);
    mat->m = m;
    mat->n = n;
    mat->nnz = nnz;
#ifdef TYPE_REAL
    if ((mat->row_idx = (int *)malloc(nnz * sizeof(int))) == NULL ||
        (mat->col_idx = (int *)malloc(nnz * sizeof(int))) == NULL ||
        (mat->val = (double *)malloc(nnz * sizeof(double))) == NULL)
    {
        fprintf(stderr, "Memory allocation failed! - coo matrix data\n");
        exit(EXIT_FAILURE);
    }

    for (int index = 0; index < nnz; ++index)
    {
        fscanf(fp, "%d%d%lf", mat->row_idx + index, mat->col_idx + index, mat->val + index);
    }
#elif defined(TYPE_COMPLEX)
    if ((mat->row_idx = (int *)malloc(nnz * sizeof(int))) == NULL ||
        (mat->col_idx = (int *)malloc(nnz * sizeof(int))) == NULL ||
        (mat->val_re = (double *)malloc(nnz * sizeof(double))) == NULL ||
        (mat->val_im = (double *)malloc(nnz * sizeof(double))) == NULL)
    {
        fprintf(stderr, "Memory allocation failed! - coo matrix data\n");
        exit(EXIT_FAILURE);
    }

    for (int index = 0; index < nnz; ++index)
    {
        fscanf(fp, "%d%d%lf%lf", mat->row_idx + index, mat->col_idx + index,
               mat->val_re + index, mat->val_im + index);
    }
#endif

    fclose(fp);
}

void COOMatrixDataFree(MatCOO *mat)
{
    free(mat->row_idx);
    free(mat->col_idx);
#ifdef TYPE_REAL
    free(mat->val);
#elif defined(TYPE_COMPLEX)
    free(mat->val_re);
    free(mat->val_im);
#endif
}