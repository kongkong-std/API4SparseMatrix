#ifndef MAIN_H_
#define MAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define TYPE_REAL
// #define TYPE_COMPLEX

#ifdef TYPE_REAL
typedef struct
{
    int m, n, nnz;
    int *row_idx, *col_idx;
    double *val;
} MatCOO;
typedef struct
{
    /* data */
    int n, nnz;
    int *row_idx, *row_ptr, *col_idx;
    double *val;
} MatCSR;
#elif defined(TYPE_COMPLEX)
typedef struct
{
    int m, n, nnz;
    int *row_idx, *col_idx;
    double *val_re, *val_im;
} MatCOO;
typedef struct
{
    /* data */
    int n, nnz;
    int *row_idx, *row_ptr, *col_idx;
    double *val_re, *val_im;
} MatCSR;
#endif // scalar type

// function prototype
/*
 * coo type matrix sorted by column ascending order
 *     IO: MatCOO * mat
 */
void COOMatrixSortAscend(MatCOO * /*coo matrix*/);

/*
 * coo type matrix file process
 *     I: const char * path, path to coo matrix file
 *     IO: MatCOO * mat, coo matrix data
 */
void COOMatrixFileProcess(const char * /*path to file*/, MatCOO * /*coo matrix*/);

/*
 * coo type matrix output to file
 *     I: const char * path, path to coo matrix file
 *     I: const MatCOO * mat, coo type matrix data
 */
void COOMatrixFileOutput(const char * /*path to file*/, const MatCOO * /*coo matrix*/);

/*
 * coo type matrix free memory
 *     IO: MatCOO * mat, coo type matrix data
 */
void COOMatrixDataFree(MatCOO * /*coo matrix*/);

/*
 * coo type matrix transform to csr type matrix
 *     I: const MatCOO * mat_coo, coo type matrix data
 *     I: int base_coo, coo type matrix file base, 0-base or 1-base
 *     IO: MatCSR * mat_csr, csr type matrix data
 *     I: int base_csr, csr type matrix file base, 0-base or 1-base
 */
void COOMatrix2CSRMatrix(const MatCOO * /*coo matrix*/, int /*coo base type*/,
                         MatCSR * /*csr matrix*/, int /*csr base type*/);

/*
 * csr type matrix output to file
 *     I: const char * path, path to csr matrix file
 *     I: const MatCSR * mat_csr, csr type matrix data
 */
void CSRMatrixFileOutput(const char * /*path to file*/, const MatCSR * /*csrmatrix*/);

/*
 * csr type matrix free memory
 *     IO: MatCSR * mat, csr type matrix data
 */
void CSRMatrixDataFree(MatCSR * /*csr matrix*/);

#endif // main.h