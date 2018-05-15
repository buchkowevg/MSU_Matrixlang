#ifndef MATRIX_EX_H
#define MATRIX_EX_H
#include "matrix.h"
#include "sparse_ex.h"
#define M_ALLOC_ERR 1
#define M_UNABLE_TO_OPEN 2
#define M_NO_MATRIX 3
#define M_NO_LENGTH 6
#define M_BAD_FORMAT 4
#define M_ZERO_INDEX 5
#define M_F_OUT_OF_RANGE 7
#define M_OUT_OF_RANGE 8
#define M_BAD_SIZES 9
class Matrix_ex : public Sparse_ex
{
public:
    Matrix_ex(unsigned int c, const Matrix *mat1 = 0, const Matrix *mat2 = 0, const char* m = "");
    const char * what () const;
    const Matrix *obj1;
    const Matrix *obj2;
private:
    String msg;
    unsigned int code;

};

#endif // MATRIX_EX_H

