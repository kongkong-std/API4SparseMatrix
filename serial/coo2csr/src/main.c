#include "main.h"

int main(int argc, char **argv)
{
    char *path_coo = NULL, *dst_coo = NULL, *dst_csr = NULL;
    int base_coo = 0, base_csr = 0;
    for (int index = 0; index < argc; ++index)
    {
        if (strstr("-path_coo", argv[index]))
        {
            // path to coo file
            path_coo = argv[index + 1];
        }
        if (strstr("-dst_coo", argv[index]))
        {
            // path to coo output
            dst_coo = argv[index + 1];
        }
        if (strstr("-dst_csr", argv[index]))
        {
            // path to csr output
            dst_csr = argv[index + 1];
        }
        if (strstr("-base_coo", argv[index]))
        {
            // coo base type
            base_coo = atoi(argv[index + 1]);
        }
        if (strstr("-base_csr", argv[index]))
        {
            // csr base type
            base_csr = atoi(argv[index + 1]);
        }
    }

#ifdef TYPE_REAL
    printf("scalar type: real\n");
#elif defined(TYPE_COMPLEX)
    printf("scalar type: complex\n");
#endif

    MatCOO mat_coo;
    MatCSR mat_csr;

    COOMatrixFileProcess(path_coo, &mat_coo);
    COOMatrixSortAscend(&mat_coo);
    COOMatrix2CSRMatrix(&mat_coo, base_coo, &mat_csr, base_csr);
    CSRMatrixFileOutput(dst_csr, &mat_csr);
    COOMatrixFileOutput(dst_coo, &mat_coo);

    // free memory
    COOMatrixDataFree(&mat_coo);
    CSRMatrixDataFree(&mat_csr);

    return 0;
}

// command line
/*
 * ./app_exe -path_coo <str1> -dst_coo <str2> -dst_csr <str3> -base_coo <0, 1> -base_csr <0, 1>
 * -path_coo ../input/mcca.mtx -dst_coo coo.txt -dst_csr csr.txt -base_coo 1 -base_csr 0
 */