#ifndef MATRIX_COORD_ROW_COLUMN
#define MATRIX_COORD_ROW_COLUMN
#include "matrix.h"
class Matrix_column_coord
{
public:
    Matrix_column_coord(Matrix*, unsigned int);
    Vector column;
    void sync_to();
    void sync_from();
private:
    unsigned int coord_index;
    Matrix* mat;
};

class Matrix_row_coord
{
public:
    Matrix_row_coord(Matrix*, unsigned int);
    Vector row;
    void sync_to();
    void sync_from();
private:
    unsigned int coord_index;
    Matrix* mat;
};

#endif // MATRIX_COORD_ROW_COLUMN

