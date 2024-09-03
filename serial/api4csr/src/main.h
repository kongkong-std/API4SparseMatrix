#ifndef MAIN_H_
#define MAIN_H_

// macro
#define ENABLE_COMPLEX

// header
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// struct
#ifdef ENABLE_COMPLEX
typedef struct scalar
{
    /* data */
    double re;
    double im;
} Scalar;
#else
typedef double Scalar;
#endif

typedef struct matrix
{
    /* data */
    int n;
    int nnz;
    int *row_idx;
    int *row_ptr;
    int *col_idx;
    Scalar *val;
} Matrix;

typedef struct vector
{
    /* data */
    int n;
    int *row_idx;
    Scalar *val;
} Vector;

// function prototype
void MatrixFileProcess(const char * /*path to file*/,
                       Matrix * /*matrix data*/);

void VectorFileProcess(const char * /*path to file*/,
                       Vector * /*vector data*/);

void MatrixFree(Matrix * /*matrix data*/);

void VectorFree(Vector * /*vector data*/);

void MatrixByVectorProduct(const Matrix * /*matrix data*/,
                           const Vector * /*vector data*/,
                           Vector * /*result vector data*/);

void VectorFileOutput(const char * /*path to file*/,
                      const Vector * /*vector data*/);

#endif