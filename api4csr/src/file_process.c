#include "main.h"

void MatrixFileProcess(const char *path, Matrix *mat)
{
    FILE *fp = NULL;
    if ((fp = fopen(path, "rb")) == NULL)
    {
        fprintf(stderr, "Cannot open file! - \'\'\n");
        exit(EXIT_FAILURE);
    }

    int n = 0, nnz = 0;
    fscanf(fp, "%d%d", &n, &nnz);
    mat->n = n;
    mat->nnz = nnz;
    if ((mat->row_idx = (int *)malloc(n * sizeof(int))) == NULL ||
        (mat->row_ptr = (int *)malloc((n + 1) * sizeof(int))) == NULL ||
        (mat->col_idx = (int *)malloc(nnz * sizeof(int))) == NULL ||
        (mat->val = (Scalar *)malloc(nnz * sizeof(Scalar))) == NULL)
    {
        fprintf(stderr, "Memory allocation failed! - \'matrix data\'\n");
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
#ifdef ENABLE_COMPLEX
        fscanf(fp, "%lf%lf", &(mat->val[index].re), &(mat->val[index].im));
#else
        fscanf(fp, "%lf", mat->val + index);
#endif
    }

    fclose(fp);
}

void VectorFileProcess(const char *path, Vector *vec)
{
    FILE *fp = NULL;
    if ((fp = fopen(path, "rb")) == NULL)
    {
        fprintf(stderr, "Cannot open file! - \'vector file\'\n");
        exit(EXIT_FAILURE);
    }

    int n;
    fscanf(fp, "%d", &n);
    vec->n = n;
    if ((vec->row_idx = (int *)malloc(n * sizeof(int))) == NULL ||
        (vec->val = (Scalar *)malloc(n * sizeof(Scalar))) == NULL)
    {
        fprintf(fp, "Memory allocation failed! - \'vector data\'\n");
        exit(EXIT_FAILURE);
    }

    for (int index = 0; index < n; ++index)
    {
#ifdef ENABLE_COMPLEX
        fscanf(fp, "%d%lf%lf", vec->row_idx + index,
               &(vec->val[index].re), &(vec->val[index].im));
#else
        fscanf(fp, "%d%lf", vec->row_idx + index, vec->val + index);
#endif
    }

    fclose(fp);
}

void VectorFileOutput(const char *path, const Vector *vec)
{
    FILE *fp = NULL;
    if ((fp = fopen(path, "wb")) == NULL)
    {
        fprintf(stderr, "Cannot open file! - \'vector output file\'\n");
        exit(EXIT_FAILURE);
    }

    int n = vec->n;
    fprintf(fp, "%d\n", vec->n);

    for (int index = 0; index < n; ++index)
    {
#ifdef ENABLE_COMPLEX
        fprintf(fp, "%d\t%021.16le\t%021.16le\n", vec->row_idx[index],
                vec->val[index].re, vec->val[index].im);
#else
        fprintf(fp, "%d\t%021.16le\n", vec->row_idx[index], vec->val[index]);
#endif
    }

    fclose(fp);
}