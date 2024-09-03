#ifndef MAIN_H_
#define MAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef TYPE_REAL
typedef struct{
    int m, n, nnz;
    int * row_idx, * col_idx;
    double * val;
}MatCOO;
typedef struct
{
    /* data */
    int n, nnz;
    int * row_idx, *row_ptr, * col_idx;
    double * val;
}MatCSR;
#elif defined(TYPE_COMPLEX)
typedef struct{
    int m, n, nnz;
    int * row_idx, * col_idx;
    double * val_re, * val_im;
}MatCOO;
typedef struct
{
    /* data */
    int n, nnz;
    int * row_idx, *row_ptr, * col_idx;
    double * val_re, *val_im;
}MatCSR;
#endif    // scalar type

#endif // main.h