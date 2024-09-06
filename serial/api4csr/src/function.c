#include "main.h"
#define MAX_SIZE 1024

void MatrixDataOutputFile(const char *path, void *mat, MatType type)
{
    switch (type)
    {
    case COO_REAL:
    {
        /* code */
        MatCOOReal *matrix = (MatCOOReal *)mat;
        COORealMatrixDataOutputFile(path, matrix);
        break;
    }

    case COO_COMPLEX:
    {
        MatCOOComplex *matrix = (MatCOOComplex *)mat;
        COOComplexMatrixDataOutputFile(path, matrix);
        break;
    }

    case CSR_REAL:
    {
        MatCSRReal *matrix = (MatCSRReal *)mat;
        CSRRealMatrixDataOutputFile(path, matrix);
        break;
    }

    case CSR_COMPLEX:
    {
        MatCSRComplex *matrix = (MatCSRComplex *)mat;
        CSRComplexMatrixDataOutputFile(path, matrix);
        break;
    }
    default:
        break;
    }
}

void COORealMatrixDataOutputFile(const char *path, const MatCOOReal *mat)
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
        fprintf(fp, "%d\t%d\t%021.16le\n", mat->row_idx[index],
                mat->col_idx[index],
                mat->val[index]);
    }

    fclose(fp);
}

void COOComplexMatrixDataOutputFile(const char *path, const MatCOOComplex *mat)
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
        fprintf(fp, "%d\t%d\t%021.16le\t%021.16le\n", mat->row_idx[index],
                mat->col_idx[index],
                mat->val_re[index],
                mat->val_im[index]);
    }

    fclose(fp);
}

void CSRRealMatrixDataOutputFile(const char *path, const MatCSRReal *mat)
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
        fprintf(fp, "%021.16le\n", mat->val[index]);
    }

    fclose(fp);
}

void CSRComplexMatrixDataOutputFile(const char *path, const MatCSRComplex *mat)
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
        fprintf(fp, "%021.16le\t%021.16le\n", mat->val_re[index], mat->val_im[index]);
    }

    fclose(fp);
}

MatCOOReal *COORealMatrix4SubAssemble(const MatCOOReal *mat_11,
                                      const MatCOOReal *mat_12,
                                      const MatCOOReal *mat_21,
                                      const MatCOOReal *mat_22)
{
    MatCOOReal *mat_tmp;

    if ((mat_tmp = (MatCOOReal *)malloc(sizeof(MatCOOReal))) == NULL)
    {
        fprintf(stderr, "Memory allocation failed! - coo real 4 sub matrices assembling\n");
        exit(EXIT_FAILURE);
    }

    int m_11 = mat_11->m, m_21 = mat_21->m;
    int n_11 = mat_11->n, n_12 = mat_12->n;
    int nnz_11 = mat_11->nnz, nnz_12 = mat_12->nnz, nnz_21 = mat_21->nnz, nnz_22 = mat_22->nnz;
    int m = m_11 + m_21;
    int n = n_11 + n_12;
    int nnz = nnz_11 + nnz_12 + nnz_21 + nnz_22;
    mat_tmp->m = m;
    mat_tmp->n = n;
    mat_tmp->nnz = nnz;

    if ((mat_tmp->row_idx = (int *)malloc(nnz * sizeof(int))) == NULL ||
        (mat_tmp->col_idx = (int *)malloc(nnz * sizeof(int))) == NULL ||
        (mat_tmp->val = (double *)malloc(nnz * sizeof(double))) == NULL)
    {
        fprintf(stderr, "Memoey allocation failed - coo complex real part data\n");
        exit(EXIT_FAILURE);
    }

    // submatrix m_11
    memcpy(mat_tmp->row_idx, mat_11->row_idx, nnz_11 * sizeof(int));
    memcpy(mat_tmp->col_idx, mat_11->col_idx, nnz_11 * sizeof(int));
    memcpy(mat_tmp->val, mat_11->val, nnz * sizeof(double));

    // submatrix m_12
    memcpy(mat_tmp->row_idx + nnz_11, mat_12->row_idx, nnz_12 * sizeof(int));
    memcpy(mat_tmp->val + nnz_11, mat_12->val, nnz_12 * sizeof(double));
    for (int index = 0; index < nnz_12; ++index)
    {
        mat_tmp->col_idx[nnz_11 + index] = mat_12->col_idx[index] + n_11;
    }

    // submatrix m_21
    memcpy(mat_tmp->col_idx + nnz_11 + nnz_12, mat_21->col_idx, nnz_21 * sizeof(int));
    memcpy(mat_tmp->val + nnz_11 + nnz_12, mat_21->val, nnz_21 * sizeof(double));
    for (int index = 0; index < nnz_21; ++index)
    {
        mat_tmp->row_idx[nnz_11 + nnz_12 + index] = mat_21->row_idx[index] + m_11;
    }

    // submatrix m_22
    memcpy(mat_tmp->val + nnz_11 + nnz_12 + nnz_21, mat_22->val, nnz_22 * sizeof(double));
    for (int index = 0; index < nnz_22; ++index)
    {
        mat_tmp->row_idx[nnz_11 + nnz_12 + nnz_21 + index] = mat_22->row_idx[index] + m_11;
        mat_tmp->col_idx[nnz_11 + nnz_12 + nnz_21 + index] = mat_22->col_idx[index] + n_11;
    }

    return mat_tmp;
}

MatCSRReal *CSRRealMatrix4SubAssemble(const MatCSRReal *mat_11,
                                      const MatCSRReal *mat_12,
                                      const MatCSRReal *mat_21,
                                      const MatCSRReal *mat_22)
{
    MatCSRReal *mat_tmp;

    if ((mat_tmp = (MatCSRReal *)malloc(sizeof(MatCSRReal))) == NULL)
    {
        fprintf(stderr, "Memory allocation failed! - csr real 4 sub matrices assembling\n");
        exit(EXIT_FAILURE);
    }

    int n_11 = mat_11->n, n_12 = mat_12->n;
    int nnz_11 = mat_11->nnz, nnz_12 = mat_12->nnz, nnz_21 = mat_21->nnz, nnz_22 = mat_22->nnz;
    int n = n_11 + n_12;
    int nnz = nnz_11 + nnz_12 + nnz_21 + nnz_22;
    mat_tmp->n = n;
    mat_tmp->nnz = nnz;
    if ((mat_tmp->row_idx = (int *)malloc(n * sizeof(int))) == NULL ||
        (mat_tmp->row_ptr = (int *)malloc((n + 1) * sizeof(int))) == NULL ||
        (mat_tmp->col_idx = (int *)malloc(nnz * sizeof(int))) == NULL ||
        (mat_tmp->val = (double *)malloc(nnz * sizeof(double))) == NULL)
    {
        fprintf(stderr, "Memory allocation failed - csr complex real part data\n");
        exit(EXIT_FAILURE);
    }

    memcpy(mat_tmp->row_idx, mat_11->row_idx, n_11 * sizeof(int));
    for (int index = 0; index < n_12; ++index)
    {
        mat_tmp->row_idx[n_11 + index] = mat_21->row_idx[index] + n_11;
    }

    int base = mat_11->row_ptr[0];
    int cnt_nnz = 0;

    // m_11, m_12
    for (int index = 0; index < n_11; ++index)
    {
        mat_tmp->row_ptr[index] = base + cnt_nnz;

        // m_11 data
        for (int index_j = mat_11->row_ptr[index]; index_j < mat_11->row_ptr[index + 1]; ++index_j)
        {
            mat_tmp->col_idx[cnt_nnz] = mat_11->col_idx[index_j - base];
            mat_tmp->val[cnt_nnz] = mat_11->val[index_j - base];
            ++cnt_nnz;
        }

        // m_12 data
        for (int index_j = mat_12->row_ptr[index]; index_j < mat_12->row_ptr[index + 1]; ++index_j)
        {
            mat_tmp->col_idx[cnt_nnz] = mat_12->col_idx[index_j - base] + n_11;
            mat_tmp->val[cnt_nnz] = mat_12->val[index_j - base];
            ++cnt_nnz;
        }
    }

    // m_21, m_22
    for (int index = 0; index < n_11; ++index)
    {
        mat_tmp->row_ptr[n_11 + index] = base + cnt_nnz;

        // m_21 data
        for (int index_j = mat_21->row_ptr[index]; index_j < mat_21->row_ptr[index + 1]; ++index_j)
        {
            mat_tmp->col_idx[cnt_nnz] = mat_21->col_idx[index_j - base];
            mat_tmp->val[cnt_nnz] = mat_21->val[index_j - base];
            ++cnt_nnz;
        }

        // m_22 data
        for (int index_j = mat_22->row_ptr[index]; index_j < mat_22->row_ptr[index + 1]; ++index_j)
        {
            mat_tmp->col_idx[cnt_nnz] = mat_22->col_idx[index_j - base] + n_11;
            mat_tmp->val[cnt_nnz] = mat_22->val[index_j - base];
            ++cnt_nnz;
        }
    }

    mat_tmp->row_ptr[n] = cnt_nnz + base;

    return mat_tmp;
}

MatCSRReal *CSRRealMatrixScale(const MatCSRReal *mat, double a)
{
    MatCSRReal *mat_tmp;

    if ((mat_tmp = (MatCSRReal *)malloc(sizeof(MatCSRReal))) == NULL)
    {
        fprintf(stderr, "Memory allocation failed! - csr real matrix scaling\n");
        exit(EXIT_FAILURE);
    }

    int n = mat->n, nnz = mat->nnz;
    mat_tmp->n = n;
    mat_tmp->nnz = nnz;
    if ((mat_tmp->row_idx = (int *)malloc(n * sizeof(int))) == NULL ||
        (mat_tmp->row_ptr = (int *)malloc((n + 1) * sizeof(int))) == NULL ||
        (mat_tmp->col_idx = (int *)malloc(nnz * sizeof(int))) == NULL ||
        (mat_tmp->val = (double *)malloc(nnz * sizeof(double))) == NULL)
    {
        fprintf(stderr, "Memory allocation failed - csr complex real part data\n");
        exit(EXIT_FAILURE);
    }

    memcpy(mat_tmp->row_idx, mat->row_idx, n * sizeof(int));
    memcpy(mat_tmp->row_ptr, mat->row_ptr, (n + 1) * sizeof(int));
    memcpy(mat_tmp->col_idx, mat->col_idx, nnz * sizeof(int));
    for (int index = 0; index < nnz; ++index)
    {
        mat_tmp->val[index] = a * mat->val[index];
    }

    return mat_tmp;
}

MatCOOReal *COORealMatrixScale(const MatCOOReal *mat, double a)
{
    MatCOOReal *mat_tmp;

    if ((mat_tmp = (MatCOOReal *)malloc(sizeof(MatCOOReal))) == NULL)
    {
        fprintf(stderr, "Memory allocation failed! - coo real matrix scaling\n");
        exit(EXIT_FAILURE);
    }

    int m = mat->m, n = mat->n, nnz = mat->nnz;
    mat_tmp->m = m;
    mat_tmp->n = n;
    mat_tmp->nnz = nnz;
    if ((mat_tmp->row_idx = (int *)malloc(nnz * sizeof(int))) == NULL ||
        (mat_tmp->col_idx = (int *)malloc(nnz * sizeof(int))) == NULL ||
        (mat_tmp->val = (double *)malloc(nnz * sizeof(double))) == NULL)
    {
        fprintf(stderr, "Memoey allocation failed - coo complex real part data\n");
        exit(EXIT_FAILURE);
    }

    memcpy(mat_tmp->row_idx, mat->row_idx, nnz * sizeof(int));
    memcpy(mat_tmp->col_idx, mat->col_idx, nnz * sizeof(int));
    for (int index = 0; index < nnz; ++index)
    {
        mat_tmp->val[index] = a * mat->val[index];
    }

    return mat_tmp;
}

MatCSRReal *CSRComplexMatrixRealPart(const MatCSRComplex *mat)
{
    MatCSRReal *mat_tmp;

    if ((mat_tmp = (MatCSRReal *)malloc(sizeof(MatCSRReal))) == NULL)
    {
        fprintf(stderr, "Memory allocation failed! - csr complex matrix real part\n");
        exit(EXIT_FAILURE);
    }

    int n = mat->n, nnz = mat->nnz;
    mat_tmp->n = n;
    mat_tmp->nnz = nnz;
    if ((mat_tmp->row_idx = (int *)malloc(n * sizeof(int))) == NULL ||
        (mat_tmp->row_ptr = (int *)malloc((n + 1) * sizeof(int))) == NULL ||
        (mat_tmp->col_idx = (int *)malloc(nnz * sizeof(int))) == NULL ||
        (mat_tmp->val = (double *)malloc(nnz * sizeof(double))) == NULL)
    {
        fprintf(stderr, "Memory allocation failed - csr complex real part data\n");
        exit(EXIT_FAILURE);
    }

    memcpy(mat_tmp->row_idx, mat->row_idx, n * sizeof(int));
    memcpy(mat_tmp->row_ptr, mat->row_ptr, (n + 1) * sizeof(int));
    memcpy(mat_tmp->col_idx, mat->col_idx, nnz * sizeof(int));
    memcpy(mat_tmp->val, mat->val_re, nnz * sizeof(double));

    return mat_tmp;
}

MatCSRReal *CSRComplexMatrixImaginaryPart(const MatCSRComplex *mat)
{
    MatCSRReal *mat_tmp;

    if ((mat_tmp = (MatCSRReal *)malloc(sizeof(MatCSRReal))) == NULL)
    {
        fprintf(stderr, "Memory allocation failed! - csr complex matrix imaginary part\n");
        exit(EXIT_FAILURE);
    }

    int n = mat->n, nnz = mat->nnz;
    mat_tmp->n = n;
    mat_tmp->nnz = nnz;
    if ((mat_tmp->row_idx = (int *)malloc(n * sizeof(int))) == NULL ||
        (mat_tmp->row_ptr = (int *)malloc((n + 1) * sizeof(int))) == NULL ||
        (mat_tmp->col_idx = (int *)malloc(nnz * sizeof(int))) == NULL ||
        (mat_tmp->val = (double *)malloc(nnz * sizeof(double))) == NULL)
    {
        fprintf(stderr, "Memory allocation failed - csr complex imaginary part data\n");
        exit(EXIT_FAILURE);
    }

    memcpy(mat_tmp->row_idx, mat->row_idx, n * sizeof(int));
    memcpy(mat_tmp->row_ptr, mat->row_ptr, (n + 1) * sizeof(int));
    memcpy(mat_tmp->col_idx, mat->col_idx, nnz * sizeof(int));
    memcpy(mat_tmp->val, mat->val_im, nnz * sizeof(double));

    return mat_tmp;
}

MatCOOReal *COOComplexMatrixRealPart(const MatCOOComplex *mat)
{
    MatCOOReal *mat_tmp;

    if ((mat_tmp = (MatCOOReal *)malloc(sizeof(MatCOOReal))) == NULL)
    {
        fprintf(stderr, "Memory allocation failed! - coo complex matrix real part\n");
        exit(EXIT_FAILURE);
    }

    int m = mat->m, n = mat->n, nnz = mat->nnz;
    mat_tmp->m = m;
    mat_tmp->n = n;
    mat_tmp->nnz = nnz;
    if ((mat_tmp->row_idx = (int *)malloc(nnz * sizeof(int))) == NULL ||
        (mat_tmp->col_idx = (int *)malloc(nnz * sizeof(int))) == NULL ||
        (mat_tmp->val = (double *)malloc(nnz * sizeof(double))) == NULL)
    {
        fprintf(stderr, "Memoey allocation failed - coo complex real part data\n");
        exit(EXIT_FAILURE);
    }

    memcpy(mat_tmp->row_idx, mat->row_idx, nnz * sizeof(int));
    memcpy(mat_tmp->col_idx, mat->col_idx, nnz * sizeof(int));
    memcpy(mat_tmp->val, mat->val_re, nnz * sizeof(double));

    return mat_tmp;
}

MatCOOReal *COOComplexMatrixImaginaryPart(const MatCOOComplex *mat)
{
    MatCOOReal *mat_tmp;

    if ((mat_tmp = (MatCOOReal *)malloc(sizeof(MatCOOReal))) == NULL)
    {
        fprintf(stderr, "Memory allocation failed! - coo complex matrix imaginary part\n");
        exit(EXIT_FAILURE);
    }

    int m = mat->m, n = mat->n, nnz = mat->nnz;
    mat_tmp->m = m;
    mat_tmp->n = n;
    mat_tmp->nnz = nnz;
    if ((mat_tmp->row_idx = (int *)malloc(nnz * sizeof(int))) == NULL ||
        (mat_tmp->col_idx = (int *)malloc(nnz * sizeof(int))) == NULL ||
        (mat_tmp->val = (double *)malloc(nnz * sizeof(double))) == NULL)
    {
        fprintf(stderr, "Memoey allocation failed - coo complex imaginary part data\n");
        exit(EXIT_FAILURE);
    }

    memcpy(mat_tmp->row_idx, mat->row_idx, nnz * sizeof(int));
    memcpy(mat_tmp->col_idx, mat->col_idx, nnz * sizeof(int));
    memcpy(mat_tmp->val, mat->val_im, nnz * sizeof(double));

    return mat_tmp;
}

void CSRComplexMatrixFileProcess(const char *path, MatCSRComplex *mat)
{
    FILE *fp = NULL;
    if ((fp = fopen(path, "rb")) == NULL)
    {
        fprintf(stderr, "Cannot open file! - \'%s\'\n", path);
        exit(EXIT_FAILURE);
    }

    int n = 0, nnz = 0;
    fscanf(fp, "%d%d", &n, &nnz);
    mat->n = n;
    mat->nnz = nnz;

    if ((mat->row_idx = (int *)malloc(n * sizeof(int))) == NULL ||
        (mat->row_ptr = (int *)malloc((n + 1) * sizeof(int))) == NULL ||
        (mat->col_idx = (int *)malloc(nnz * sizeof(int))) == NULL ||
        (mat->val_re = (double *)malloc(nnz * sizeof(double))) == NULL ||
        (mat->val_im = (double *)malloc(nnz * sizeof(double))) == NULL)
    {
        fprintf(stderr, "Memory allocation failed! - complex csr matrix file\n");
        exit(EXIT_FAILURE);
    }

    for (int index = 0; index < n; ++index)
    {
        fscanf(fp, "%d", mat->row_idx + index);
    }
    for (int index = 0; index < n + 1; ++index)
    {
        fscanf(fp, "%d", mat->row_ptr + index);
    }
    for (int index = 0; index < nnz; ++index)
    {
        fscanf(fp, "%d", mat->col_idx + index);
    }
    for (int index = 0; index < nnz; ++index)
    {
        fscanf(fp, "%lf%lf", mat->val_re + index,
               mat->val_im + index);
    }

    fclose(fp);
}

void COOComplexMatrixFileProcess(const char *path, MatCOOComplex *mat)
{
    FILE *fp = NULL;
    if ((fp = fopen(path, "rb")) == NULL)
    {
        fprintf(stderr, "Cannot open file! - \'%s\'\n", path);
        exit(EXIT_FAILURE);
    }

    int m = 0, n = 0, nnz = 0;
    char buffer[MAX_SIZE];
    while (fgets(buffer, MAX_SIZE, fp) != NULL)
    {
        /* code */
        if (buffer[0] != '%')
        {
            break;
        }
    }

    sscanf(buffer, "%d%d%d", &m, &n, &nnz);
    mat->m = m;
    mat->n = n;
    mat->nnz = nnz;

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

    fclose(fp);
}

void CSRComplexMatrixDataFree(MatCSRComplex *mat)
{
    free(mat->row_idx);
    free(mat->row_ptr);
    free(mat->col_idx);
    free(mat->val_re);
    free(mat->val_im);
}

void COOComplexMatrixDataFree(MatCOOComplex *mat)
{
    free(mat->row_idx);
    free(mat->col_idx);
    free(mat->val_re);
    free(mat->val_im);
}

void CSRRealMatrixDataFree(MatCSRReal *mat)
{
    free(mat->row_idx);
    free(mat->row_ptr);
    free(mat->col_idx);
    free(mat->val);
}

void COORealMatrixDataFree(MatCOOReal *mat)
{
    free(mat->row_idx);
    free(mat->col_idx);
    free(mat->val);
}