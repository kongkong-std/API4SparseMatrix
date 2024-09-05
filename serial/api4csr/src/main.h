#ifndef MAIN_H_
#define MAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    /* data */
    int m, n, nnz;
    int *row_idx, *col_idx;
    double *val;
} MatCOOReal;

typedef struct
{
    /* data */
    int m, n, nnz;
    int *row_idx, *col_idx;
    double *val_re, *val_im;
} MatCOOComplex;

typedef struct
{
    /* data */
    int n, nnz;
    int *row_idx, *row_ptr, *col_idx;
    double *val;
} MatCSRReal;

typedef struct
{
    /* data */
    int n, nnz;
    int *row_idx, *row_ptr, *col_idx;
    double *val_re, *val_im;
} MatCSRComplex;

typedef enum
{
    COO_REAL,
    COO_COMPLEX,
    CSR_REAL,
    CSR_COMPLEX
} MatType;

// function prototype
/*
 * csr type complex matrix file process
 *     I: const char * path, path to csr matrix file
 *     IO: MatCSRComplex * mat, csr complex matrix data
 */
void CSRComplexMatrixFileProcess(const char * /*path to file*/, MatCSRComplex * /*csr complex matrix*/);

/*
 * coo type complex matrix file process
 *     I: const char * path, path to coo matrix file
 *     IO: MatCOOComplex * mat, coo complex matrix data
 */
void COOComplexMatrixFileProcess(const char * /*path to file*/, MatCOOComplex * /*coo complex matrix*/);

/*
 * csr type complex matrix data free
 *     I: MatCSRComplex * mat, csr complex matrix data
 */
void CSRComplexMatrixDataFree(MatCSRComplex * /*csr complex matrix*/);

/*
 * coo type complex matrix data free
 *     I: MatCOOComplex * mat, coo complex matrix data
 */
void COOComplexMatrixDataFree(MatCOOComplex * /*coo complex matrix*/);

/*
 * csr type real matrix data free
 *     I: MatCSRReal * mat, csr real matrix data
 */
void CSRRealMatrixDataFree(MatCSRReal * /*csr real matrix*/);

/*
 * coo type real matrix data free
 *     I: MatCOOReal * mat, coo real matrix data
 */
void COORealMatrixDataFree(MatCOOReal * /*coo real matrix*/);

/*
 * real part of csr complex matrix
 *     I: const MatCSRComplex * mat, csr complex matrix
 *     R: MatCSRReal * type data, real part
 */
MatCSRReal *CSRComplexMatrixRealPart(const MatCSRComplex * /*csr complex matrix*/);

/*
 * imaginary part of csr complex matrix
 *     I: const MatCSRComplex * mat, csr complex matrix
 *     R: MatCSRReal * type data, imaginary part
 */
MatCSRReal *CSRComplexMatrixImaginaryPart(const MatCSRComplex * /*csr complex matrix*/);

/*
 * real part of coo complex matrix
 *     I: const MatCOOComplex * mat, coo complex matrix
 *     R: MatCOOReal * type data, real part
 */
MatCOOReal *COOComplexMatrixRealPart(const MatCOOComplex * /*coo complex matrix*/);

/*
 * imaginary part of coo complex matrix
 *     I: const MatCOOComplex * mat, coo complex matrix
 *     R: MatCOOReal * type data, imaginary part
 */
MatCOOReal *COOComplexMatrixImaginaryPart(const MatCOOComplex * /*coo complex matrix*/);

/*
 * csr real matrix scaling with a
 *     I: const MatCSRReal * mat, csr real matrix
 *     I: double a, scaling
 *     R: MatCSRReal * type data, mat scaling with a
 */
MatCSRReal *CSRRealMatrixScale(const MatCSRReal * /*csr real matrix*/, double /*scaling*/);

/*
 * coo real matrix scaling with a
 *     I: const MatCOOReal * mat, coo real matrix
 *     I: double a, scaling
 *     R: MatCOOReal * type data, mat scaling with a
 */
MatCOOReal *COORealMatrixScale(const MatCOOReal * /*csr real matrix*/, double /*scaling*/);

/*
 * 4 square coo real matrices assemble a coo matrix
 *     [ mat_11 mat_12 ]
 *     [ mat_21 mat_22 ]
 *     I: MatCOOReal * mat_11, block (1, 1) entry sub matrix
 *     I: MatCOOReal * mat_12, block (1, 2) entry sub matrix
 *     I: MatCOOReal * mat_21, block (2, 1) entry sub matrix
 *     I: MatCOOReal * mat_22, block (2, 2) entry sub matrix
 *     R: MatCOOReal * type data, assembled matrix
 */
MatCOOReal *COORealMatrix4SubAssemble(const MatCOOReal * /*coo real matrix 11*/,
                                      const MatCOOReal * /*coo real matrix 12*/,
                                      const MatCOOReal * /*coo real matrix 21*/,
                                      const MatCOOReal * /*coo real matrix 22*/);

/*
 * 4 square csr real matrices assemble a coo matrix
 *     [ mat_11 mat_12 ]
 *     [ mat_21 mat_22 ]
 *     I: MatCSRReal * mat_11, block (1, 1) entry sub matrix
 *     I: MatCSRReal * mat_12, block (1, 2) entry sub matrix
 *     I: MatCSRReal * mat_21, block (2, 1) entry sub matrix
 *     I: MatCSRReal * mat_22, block (2, 2) entry sub matrix
 *     R: MatCSRReal * type data, assembled matrix
 */
MatCSRReal *CSRRealMatrix4SubAssemble(const MatCSRReal * /*csr real matrix 11*/,
                                      const MatCSRReal * /*csr real matrix 12*/,
                                      const MatCSRReal * /*csr real matrix 21*/,
                                      const MatCSRReal * /*csr real matrix 22*/);

/*
 * matrix data output to file
 *     I: const char * path, path to file
 *     I: void * mat, matrix data
 *     I: MatType type, enumerate coo_real, coo_complex, csr_real, csr_complex
 */
void MatrixDataOutputFile(const char * /*path to file*/, void * /*matrix data*/, MatType /*matrix type*/);

/*
 * coo real matrix data output to file
 *     I: const char * path, path to file
 *     I: const MatCOOReal * mat, coo real matrix data
 */
void COORealMatrixDataOutputFile(const char * /*path to file*/, const MatCOOReal * /*coo real matrix data*/);

/*
 * coo complex matrix data output to file
 *     I: const char * path, path to file
 *     I: const MatCOOComplex * mat, coo complex matrix data
 */
void COOComplexMatrixDataOutputFile(const char * /*path to file*/, const MatCOOComplex * /*coo complex matrix data*/);

/*
 * csr real matrix data output to file
 *     I: const char * path, path to file
 *     I: const MatCSRReal * mat, csr real matrix data
 */
void CSRRealMatrixDataOutputFile(const char * /*path to file*/, const MatCSRReal * /*csr real matrix data*/);

/*
 * csr complex matrix data output to file
 *     I: const char * path, path to file
 *     I: const MatCSRComplex * mat, csr complex matrix data
 */
void CSRComplexMatrixDataOutputFile(const char * /*path to file*/, const MatCSRComplex * /*csr complex matrix data*/);

#endif // main.h