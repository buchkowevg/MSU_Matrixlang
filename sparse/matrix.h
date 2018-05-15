#ifndef MATRIX_H
#define MATRIX_H
#include "matrix_coord.h"
#include "rational_number.h"
#include "vector.h"
#include <stdlib.h> 
struct mnode // структура для представления узлов дерева
{
    Matrix_coord key;
    unsigned char height;
    Rational_number data;
    mnode* left;
    mnode* right;
    mnode(Matrix_coord k, Rational_number d)
    { key = k; left = right = 0; height = 1; data = d;}
};
class Matrix
{
    class Matrix_support
    {
        public:
            Matrix_support(const Matrix *m, unsigned int coord)
            {
                mat = m;
                i = coord;
            }
            Rational_number operator[](unsigned int j) const
            {
                 return mat->operator [](Matrix_coord(i,j));
            }
        private:
            unsigned int i;
            const Matrix *mat;
    };
public:
    Matrix();
    Matrix(const Matrix&);
    Matrix(const Vector&, bool = 0);
    Matrix(unsigned int, const Rational_number& = 0, bool = 0);
    Matrix(unsigned int, unsigned int,const Rational_number& = 0);
    Matrix(const char*);
    ~Matrix();
    Matrix& operator =(const Matrix&);
    Matrix operator -() const;
    Matrix operator ~() const;
    Matrix operator ^(unsigned int) const;
    friend Vector operator * (const Matrix&, const Vector&);
    friend Matrix operator + (const Matrix&, const Matrix&);
    friend Matrix operator - (const Matrix&, const Matrix&);
    friend Matrix operator * (const Matrix&, const Matrix&);
    friend Matrix operator + (const Rational_number&, const Matrix&);
    friend Matrix operator - (const Rational_number&, const Matrix&);
    friend Matrix operator * (const Rational_number&, const Matrix&);
    friend Matrix operator / (const Rational_number&, const Matrix&);
    friend char* to_string(const Matrix&);
    Matrix_support operator [](unsigned int i) const
    {
        return Matrix_support(this, i);
    }
    Rational_number operator [](Matrix_coord) const;
    Rational_number& operator() (unsigned int, unsigned int);
    Rational_number& operator() (Matrix_coord);
    char* get_str() const;
    unsigned int get_length_x() const;
    unsigned int get_length_y() const;
    Vector get_column(unsigned int) const;
    Vector get_row(unsigned int) const;
    void write(const char*) const;
    Matrix insert_row(const Vector&, unsigned int);
    Matrix insert_column(const Vector&, unsigned int);
    Matrix make_canonical();
    friend std::ostream& operator << (std::ostream&,const Matrix&);
private:
    unsigned int length_x;
    unsigned int length_y;
    mnode* tree;

    //tree functions
    unsigned char height(mnode* p) const;
    int bfactor(mnode* p) const;
    void fixheight(mnode *p);
    Rational_number& get_elemref(mnode *p, Matrix_coord k);
    Rational_number get_elem(mnode *p, Matrix_coord k) const;
    mnode* rotateright(mnode* p);
    mnode* rotateleft(mnode* q);
    mnode* balance(mnode* p);
    mnode* insert(mnode* p, Matrix_coord k, Rational_number d);
    mnode* findmin(mnode* p);
    mnode* removemin(mnode* p);
    mnode* remove(mnode* p, Matrix_coord k);
    mnode* remove(mnode* p);
};


#endif // MATRIX_H
