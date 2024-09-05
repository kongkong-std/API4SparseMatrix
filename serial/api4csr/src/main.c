#include "main.h"

int main(int argc, char **argv)
{
    char *path_mat = NULL, *dst_mat = NULL;
    char *dst_re = NULL, *dst_im = NULL;
    for (int index = 0; index < argc; ++index)
    {
        if (strstr("-path_mat", argv[index]))
        {
            path_mat = argv[index + 1];
        }
        if (strstr("-dst_mat", argv[index]))
        {
            dst_mat = argv[index + 1];
        }
        if (strstr("-dst_re", argv[index]))
        {
            dst_re = argv[index + 1];
        }
        if (strstr("-dst_im", argv[index]))
        {
            dst_im = argv[index + 1];
        }
    }

    MatCOOComplex mat;
    COOComplexMatrixFileProcess(path_mat, &mat);

    MatCOOReal *mat_re = COOComplexMatrixRealPart(&mat);
    MatCOOReal *mat_im = COOComplexMatrixImaginaryPart(&mat);
    MatCOOReal *minus_mat_im = COORealMatrixScale(mat_im, -1.);

    MatCOOReal *mat_block = COORealMatrix4SubAssemble(mat_re, minus_mat_im, mat_im, mat_re);

    MatrixDataOutputFile(dst_re, mat_re, COO_REAL);
    MatrixDataOutputFile(dst_im, mat_im, COO_REAL);
    MatrixDataOutputFile(dst_mat, mat_block, COO_REAL);

    // free memory
    COOComplexMatrixDataFree(&mat);
    COORealMatrixDataFree(mat_re);
    free(mat_re);
    COORealMatrixDataFree(mat_im);
    free(mat_im);
    COORealMatrixDataFree(minus_mat_im);
    free(minus_mat_im);
    COORealMatrixDataFree(mat_block);
    free(mat_block);

    return 0;
}

// command line
/*
 * ./app_exe -path_mat <str1> -dst_mat <str2> -dst_re <str3> -dst_im <str4>
 * ./app_exe -path_mat ../input/young1c.mtx -dst_mat coo_block.txt -dst_re coo_re.txt -dst_im coo_im.txt
 */