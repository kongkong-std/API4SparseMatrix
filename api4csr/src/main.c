#include "main.h"

int main(int argc, char **argv)
{
    char *path_mat = NULL,
         *path_vec = NULL,
         *path_result = NULL;
    for (int index = 0; index < argc; ++index)
    {
        if (strstr("-mat", argv[index]))
        {
            path_mat = argv[index + 1];
        }
        if (strstr("-vec", argv[index]))
        {
            path_vec = argv[index + 1];
        }
        if (strstr("-result", argv[index]))
        {
            path_result = argv[index + 1];
        }
    }

    Matrix mat;
    Vector vec_x, vec_y;

    MatrixFileProcess(path_mat, &mat);
    VectorFileProcess(path_vec, &vec_x);

    // vec_y = mat x vec_x
    MatrixByVectorProduct(&mat, &vec_x, &vec_y);

    // file IO
    VectorFileOutput(path_result, &vec_y);

    // free memory
    MatrixFree(&mat);
    VectorFree(&vec_x);
    VectorFree(&vec_y);

    return 0;
}