#include "main.h"

int main(int argc, char **argv)
{
    char *path_coo = NULL,
         *path_csr = NULL;
    for (int index = 0; index < argc; ++index)
    {
        if (strstr("-csr", argv[index]))
        {
            path_csr = argv[index + 1];
        }
        if (strstr("-coo", argv[index]))
        {
            path_coo = argv[index + 1];
        }
    }

    Coo mat_coo;
    Csr mat_csr;

    CooFileProcess(path_coo, &mat_coo);
    Coo2Csr(&mat_coo, &mat_csr);
    CsrFileProcess(path_csr, &mat_csr);

    // free memory
    CooMatrixFree(&mat_coo);
    CsrMatrixFree(&mat_csr);

    return 0;
}