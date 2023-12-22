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

typedef struct coo
{
    /* data */
    int n;
    int nnz;
    int *row_idx;
    int *col_idx;
    Scalar *val;
} Coo;

typedef struct csr
{
    /* data */
    int n;
    int nnz;
    int *row_idx;
    int *row_ptr;
    int *col_idx;
    Scalar *val;
} Csr;

// function prototype
void CooFileProcess(const char * /*path to file*/,
                    Coo * /*coo matrix data*/);

void CsrFileProcess(const char * /*path to file*/,
                    const Csr * /*csr matrix data*/);

void CooMatrixFree(Coo * /*coo matrix data*/);

void CsrMatrixFree(Csr * /*csr matrix data*/);

void Coo2Csr(const Coo * /*coo matrix data*/,
             Csr * /*csr matrix data*/);

#endif