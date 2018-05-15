#include "matrix.h"
#include "mystring.h"
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include "matrix_ex.h"
#include "vector_ex.h"
#include "rational_number_ex.h"
using namespace std;

#define READING_NAME 0
#define WAITING_FOR_NAME 1
#define WAITING_FOR_LENGTH_X 2
#define WAITING_FOR_LENGTH_Y 12
#define READING_LENGTH_X 3
#define READING_LENGTH_Y 13
#define WAITING_FOR_ENDSHARPWON 4
#define WAITING_FOR_ENDSHARP 5
#define WAITING_FOR_KEY1 6
#define WAITING_FOR_KEY2 14
#define WAITING_FOR_ENDL 7
#define READING_KEY1 8
#define READING_KEY2 11
#define WAITING_FOR_DATA 9
#define READING_DATA 10

Matrix::Matrix()
{
    length_x = 0;
    length_y = 0;
    tree = 0;
}
Matrix::Matrix(const Matrix &obj)
{
    length_x = obj.get_length_x();
    length_y = obj.get_length_y();
    tree = 0;
    for(unsigned int i = 0; i < length_y; i++)
    {
        for(unsigned int j = 0; j < length_x; j++)
        {
            if(obj.get_elem(obj.tree, Matrix_coord(j, i)) != 0)
            {
                try
                {
                    tree = insert(tree, Matrix_coord(j, i), obj.get_elem(obj.tree, Matrix_coord(j, i)));
                }
                catch(...)
                {
                    tree = remove(tree);
                    throw Matrix_ex(M_ALLOC_ERR);
                }
            }
        }
    }
}
Matrix::Matrix(unsigned int length,const  Rational_number& rat, bool flag)
{
    length_x = length_y = length;
    tree = 0;
    if(rat == 0) return;
    try
    {
        if(flag)
        {
            for(unsigned int i = 0; i < length; i++)
            {
                for(unsigned int j = 0; j < length; j++)
                {
                    tree = insert(tree, Matrix_coord(i ,j), rat);
                }
            }
        }
        else
        {
            for(unsigned int i = 0; i < length; i++) tree = insert(tree, Matrix_coord(i, i), rat);
        }
    }
    catch(...)
    {
        tree = remove(tree);
        throw Matrix_ex(M_ALLOC_ERR);
    }
}
Matrix::Matrix(unsigned int x, unsigned int y,const  Rational_number& rat)
{
    length_x = x;
    length_y = y;
    tree = 0;
    if(rat == 0) return;
    try
    {
        for(unsigned int i = 0; i < length_y; i++)
        {
            for(unsigned int j = 0; j < length_x; j++)
            {
                tree = insert(tree, Matrix_coord(j, i), rat);
            }
        }
    }
    catch(...)
    {
        tree = remove(tree);
        throw Matrix_ex(M_ALLOC_ERR);
    }
}
Matrix::Matrix(const Vector &obj, bool flag)
{
    tree = 0;
    try
    {
        if(!flag)
        {
            length_y = 1;
            length_x = obj.get_number_of_elements();
            for(unsigned int i = 0; i < length_x; i++)
            {
                for(unsigned int j = 0; j < length_y; j++)
                {
                    if(obj[i] != 0) tree = insert(tree, Matrix_coord(i, j), obj[i]);
                }
            }
        }
        else
        {
            length_x = 1;
            length_y = obj.get_number_of_elements();
            for(unsigned int i = 0; i < length_x; i++)
            {
                for(unsigned int j = 0; j < length_y; j++)
                {
                    if(obj[j] != 0) tree = insert(tree, Matrix_coord(i, j), obj[j]);
                }
            }
        }
    }
    catch(...)
    {
        tree = remove(tree);
        throw Matrix_ex(M_ALLOC_ERR);
    }
}
Matrix::Matrix(const char* file_name)
{
    ifstream inFile;
    String str;
    tree = 0;
    char *s = 0;
    char c;
    char buf[6];
    buf[5] = '\0';
    int r = WAITING_FOR_NAME;
    int count = 4;
    unsigned int x = 0;
    unsigned int y = 0;
    length_x = 0;
    length_y = 0;
    Rational_number rat;
    inFile.open(file_name);
    if (!inFile) {
        throw Matrix_ex(M_UNABLE_TO_OPEN,0,0,file_name);
        return;
    }
    inFile.unsetf(ios_base::skipws);
    try
    {
        while(inFile >> c)
        {
            if(c == '\t') c = ' ';
            if(r == WAITING_FOR_ENDSHARPWON)
            {
               if(c == '\n')
               {
                    r = WAITING_FOR_NAME;
                    continue;
               }
               else continue;
            }
            if(r == WAITING_FOR_ENDSHARP)
            {
                if(c == '\n')
                {
                     r = WAITING_FOR_KEY1;
                     continue;
                }
                else continue;
            }
            if(r == WAITING_FOR_NAME)
            {
                if(c == ' ' || c == '\n') continue;
                if(c == '#')
                {
                    r = WAITING_FOR_ENDSHARPWON;
                    continue;
                }
                r = READING_NAME;
            }
            if(r == READING_NAME)
            {
                if(count < 0)
                {
                    if(strcmp(buf, "matri") == 0 && c == 'x')
                    {
                        r = WAITING_FOR_LENGTH_X;
                        continue;
                    }
                    else
                    {
                        throw Matrix_ex(M_NO_MATRIX,0,0,file_name);
                        break;
                    }
                }
                else
                {
                    buf[4 - count] = c;
                    count--;
                }
                continue;
            }
            if(r == WAITING_FOR_LENGTH_X)
            {
                if(c == ' ') continue;
                else
                {
                    if(c >= '0' && c <= '9') r = READING_LENGTH_X;
                    else
                    {
                        throw Matrix_ex(M_NO_LENGTH,0,0,file_name);
                        break;
                    }
                }
            }
            if(r == READING_LENGTH_X)
            {
                if(c >= '0' && c <= '9') length_x = c - '0' + length_x * 10;
                else r = WAITING_FOR_LENGTH_Y;
            }
            if(r == WAITING_FOR_LENGTH_Y)
            {
                if(c == ' ') continue;
                else
                {
                    if(c >= '0' && c <= '9') r = READING_LENGTH_Y;
                    else
                    {
                        throw Matrix_ex(M_NO_LENGTH,0,0,file_name);
                        break;
                    }
                }
            }
            if(r == READING_LENGTH_Y)
            {
                if(c >= '0' && c <= '9') length_y = c - '0' + length_y * 10;
                else r = WAITING_FOR_ENDL;
            }
            if(r == WAITING_FOR_ENDL)
            {
                if(c == '#')
                {
                    r = WAITING_FOR_ENDSHARP;
                    continue;
                }
                if(c == ' ') continue;
                if(c == '\n')
                {
                    r = WAITING_FOR_KEY1;
                    x = 0;
                    y = 0;
                    continue;
                }
                throw Matrix_ex(M_BAD_FORMAT,0,0,file_name);
                break;
            }
            if(r == WAITING_FOR_KEY1)
            {
                if(c == '#')
                {
                    r = WAITING_FOR_ENDSHARP;
                    continue;
                }
                if(c == ' ' || c == '\n') continue;
                else r = READING_KEY1;
            }
            if(r == READING_KEY1)
            {
                if(c >= '0' && c <= '9')
                {
                    x = c - '0' + x * 10;
                    continue;
                }
                else
                {
                    if(x == 0)
                    {
                        throw Matrix_ex(M_ZERO_INDEX,0,0,file_name);
                        break;
                    }
                    if(x > length_x )
                    {
                        throw Matrix_ex(M_F_OUT_OF_RANGE,0,0,file_name);
                        break;
                    }
                    r = WAITING_FOR_KEY2;
                }
            }
            if(r == WAITING_FOR_KEY2)
            {
                if(c == ' ') continue;
                else r = READING_KEY2;
            }
            if(r == READING_KEY2)
            {
                if(c >= '0' && c <= '9')
                {
                    y = c - '0' + y * 10;
                    continue;
                }
                else
                {
                    if(y == 0)
                    {
                        throw Matrix_ex(M_ZERO_INDEX,0,0,file_name);
                        break;
                    }
                    if(y > length_y )
                    {
                        throw Matrix_ex(M_F_OUT_OF_RANGE,0,0,file_name);
                        break;
                    }
                    r = WAITING_FOR_DATA;
                }
            }
            if(r == WAITING_FOR_DATA)
            {
                if(c == ' ') continue;
                else r = READING_DATA;
            }
            if(r == READING_DATA)
            {
                if(c == '\n' || c == '#')
                {
                    if(str.getlength() == 0) rat = 0;
                    else
                    {
                        s = str.getchararr();
                        rat = s;
                        free(s);
                    }
                    s = NULL;
                    if(c == '#') r = WAITING_FOR_ENDSHARP;
                    else r = WAITING_FOR_KEY1;
                    str = "";
                    if(rat != 0) tree = insert(tree, Matrix_coord(x - 1, y - 1), rat);
                    x = 0;
                    y = 0;
                    continue;
                }
                str = str + c;
            }
        }
        if(r == READING_DATA || r == WAITING_FOR_DATA)
        {
            if(str.getlength() == 0) rat = 0;
            else
            {
                s = str.getchararr();
                rat = s;
                free(s);
            }
            s = NULL;
            r = WAITING_FOR_KEY1;
            if(rat != 0) tree = insert(tree, Matrix_coord(x - 1, y - 1), rat);
            x = 0;
            y = 0;
        }
        if(r == WAITING_FOR_LENGTH_X || r == WAITING_FOR_LENGTH_Y) throw Matrix_ex(M_NO_LENGTH,0,0,file_name);
        if(r == WAITING_FOR_ENDSHARPWON || r == WAITING_FOR_NAME || r == READING_NAME)
            throw Matrix_ex(M_NO_MATRIX,0,0,file_name);
    }
    catch(const Matrix_ex& ex)
    {
        tree = remove(tree);
        length_x = 0;
        length_y = 0;
        throw ex;
    }
    catch(const Rational_number_ex& ex)
    {
        tree = remove(tree);
        length_x = 0;
        length_y = 0;
        throw ex;
    }
    catch(...)
    {
        tree = remove(tree);
        length_x = 0;
        length_y = 0;
        throw Matrix_ex(M_ALLOC_ERR);
    }

}
Matrix::~Matrix()
{
    tree = remove(tree);
}
char* Matrix::get_str() const
{
    try
    {
        String str = "";
        for(unsigned int i = 0; i < length_x; i++)
        {
            for(unsigned int j = 0; j < length_y; j++) str = str + get_elem(tree, Matrix_coord(i, j)).get_String() + ' ';
            str = str + '\n';
        }
        return str.getchararr();
    }
    catch(...)
    {
        throw Matrix_ex(M_ALLOC_ERR);
    }
}
unsigned int Matrix::get_length_x() const
{
    return length_x;
}
unsigned int Matrix::get_length_y() const
{
    return length_y;
}
Vector Matrix::get_column(unsigned int column) const
{
    if(column >= length_y) throw Matrix_ex(M_OUT_OF_RANGE, this);
    Vector res(length_x);
    Rational_number rat;
    try
    {
        for(unsigned int i = 0; i < length_x; i++)
            if((rat = get_elem(tree, Matrix_coord(i, column))) != 0)
                res(i) = rat;
    }
    catch(...)
    {
        throw Matrix_ex(M_ALLOC_ERR);
    }
    return res;
}
Vector Matrix::get_row(unsigned int row) const
{
    if(row >= length_x) throw Matrix_ex(M_OUT_OF_RANGE, this);
    Vector res(length_y);
    Rational_number rat;
    try
    {
        for(unsigned int i = 0; i < length_y; i++)
            if((rat = get_elem(tree, Matrix_coord(row, i))) != 0)
                res(i) = rat;
    }
    catch(...)
    {
        throw Matrix_ex(M_ALLOC_ERR);
    }
    return res;
}
Matrix Matrix::insert_row(const Vector& obj, unsigned int row)
{
    if(length_x <= row) throw Matrix_ex(M_OUT_OF_RANGE, this);
    if(length_y != obj.get_number_of_elements()) throw Matrix_ex(M_BAD_SIZES, this);;
    Rational_number rat;
    try
    {
        for(unsigned int i = 0; i < length_y; i++)
        {
            tree = remove(tree, Matrix_coord(row, i));
            if((rat = obj[i]) != 0)
                tree = insert(tree, Matrix_coord(row, i), rat);
        }
    }
    catch(...)
    {
        throw Matrix_ex(M_ALLOC_ERR);
    }
    return *this;
}
Matrix Matrix::insert_column(const Vector& obj, unsigned int column)
{
    if(length_y <= column) throw Matrix_ex(M_OUT_OF_RANGE, this);
    if(length_x != obj.get_number_of_elements()) throw Matrix_ex(M_BAD_SIZES, this);;
    Rational_number rat;
    try
    {
        for(unsigned int i = 0; i < length_x; i++)
        {
            tree = remove(tree, Matrix_coord(i, column));
            if((rat = obj[i]) != 0)
                tree = insert(tree, Matrix_coord(i, column), rat);
        }
    }
    catch(...)
    {
        throw Matrix_ex(M_ALLOC_ERR);
    }
    return *this;
}
Matrix Matrix::make_canonical()
{
    Rational_number res;
    for(unsigned int i = 0; i < length_x; i++)
        for(unsigned int j = 0; j < length_y; j++)
            if((res = this->operator [](i)[j]) != 0)
                this->operator ()(i, j) = res.make_canonical();
    return *this;
}
Rational_number Matrix::operator [](Matrix_coord coord) const
{
    if(coord.get_x() >= length_x || coord.get_y() >= length_y)
        throw Matrix_ex(M_OUT_OF_RANGE, this);
    return get_elem(tree, coord);
}
Rational_number& Matrix::operator ()(Matrix_coord coord)
{
    if(coord.get_x() >= length_x || coord.get_y() >= length_y)
        throw Matrix_ex(M_OUT_OF_RANGE, this);
    if(get_elem(tree, coord) == 0) tree = insert(tree, coord, 0);
    return get_elemref(tree, coord);
}
Rational_number& Matrix::operator ()(unsigned int x, unsigned int y)
{
    return this->operator ()(Matrix_coord(x, y));
}
Matrix& Matrix:: operator =(const Matrix& obj)
{
    tree = remove(tree);
    length_x = obj.get_length_x();
    length_y = obj.get_length_y();
    Rational_number rat;
    try
    {
        for(unsigned int i = 0; i < length_x; i ++)
            for(unsigned int j = 0; j < length_y; j++)
                if((rat = obj[Matrix_coord(i, j)]) != 0)
                    tree = insert(tree, Matrix_coord(i,j), rat);
    }
    catch(...)
    {
        throw Matrix_ex(M_ALLOC_ERR);
    }
    return *this;
}
Matrix Matrix::operator -() const
{
    Rational_number rat;
    Matrix res(this->get_length_x(),this->get_length_y());
    try
    {
        for(unsigned int i = 0; i < length_x; i++)
            for(unsigned int j = 0 ; j < length_y; j++)
            {
                if((rat = this->get_elem(tree,Matrix_coord(i, j))) != 0)
                    res(i , j) = -rat;
            }
    }
    catch(...)
    {
        throw Matrix_ex(M_ALLOC_ERR);
    }
    return res;
}
Matrix Matrix::operator ~() const
{
    Matrix res(length_y, length_x);
    Rational_number rat;
    try
    {
        for(unsigned int i = 0; i < length_x; i++)
            for(unsigned int j = 0; j < length_y; j++)
                if((rat = get_elem(tree, Matrix_coord(i, j))) != 0)
                    res(j , i) = rat;
    }
    catch(...)
    {
        throw Matrix_ex(M_ALLOC_ERR);
    }
    return res;
}
Matrix Matrix::operator ^(unsigned int power) const
{
    if(length_x != length_y) throw Matrix_ex(M_BAD_SIZES, this);
    try
    {
        if(power == 0) return Matrix(length_x, 1);
        Matrix temp = *this;
        Matrix res(length_x);
        Rational_number rat;
        for(unsigned int p = 1; p < power; p++)
        {
            for(unsigned int i = 0; i < res.get_length_x(); i++)
                for(unsigned int j = 0; j < res.get_length_y(); j++)
                {
                    rat = 0;
                    for(unsigned int k = 0; k < res.get_length_y(); k++)
                        rat += temp[Matrix_coord(i, k)] * this->operator [](Matrix_coord(k, j));
                    if(rat != 0) res(i ,j) = rat.make_canonical();
                    else res.tree = res.remove(res.tree, Matrix_coord(i, j));
                }
            for(unsigned int i = 0; i < res.get_length_x(); i++)
                for(unsigned int j = 0; j < res.get_length_y(); j++)
                {
                    if((rat = res[Matrix_coord(i,j)]) == 0)
                        temp.tree = temp.remove(temp.tree, Matrix_coord(i,j));
                    else temp(i, j) = rat;
                }
        }
        return res;
    }
    catch(Rational_number_ex *ex)
    {
        throw ex;
    }
    catch(...)
    {
        throw Matrix_ex(M_ALLOC_ERR);
    }
}
Vector operator *(const Matrix& arg1, const Vector& arg2)
{
    if(arg1.get_length_y() != arg2.get_number_of_elements()) throw Matrix_ex(M_BAD_SIZES, &arg1);
    Vector res(arg1.get_length_x());
    Rational_number rat;
    try
    {
       for(unsigned int i = 0; i < arg1.get_length_x(); i++)
       {
           rat = 0;
           for(unsigned int j = 0; j < arg1.get_length_y(); j++)
               rat += arg1[Matrix_coord(i,j)] * arg2[j];
           if(rat != 0) res(i) = rat.make_canonical();
       }
    }
    catch(Rational_number_ex *ex)
    {
        throw ex;
    }
    catch(...)
    {
        throw Matrix_ex(M_ALLOC_ERR);
    }

    return res;
}
Matrix operator +(const Matrix& arg1, const Matrix& arg2)
{
    if((arg1.get_length_x() != arg2.get_length_x()) ||
            (arg1.get_length_y() != arg2.get_length_y()))
                throw Matrix_ex(M_BAD_SIZES, &arg1, &arg2);
    Matrix res(arg1.get_length_x(), arg2.get_length_y());
    Rational_number rat;
    try
    {
        for(unsigned int i = 0; i < res.length_x; i++)
            for(unsigned int j = 0; j < res.length_y; j++)
                if((rat = arg1[Matrix_coord(i , j)] + arg2[Matrix_coord(i , j)]) != 0)
                    res(i , j) = rat;
    }
    catch(Rational_number_ex *ex)
    {
        throw ex;
    }
    catch(...)
    {
        throw Matrix_ex(M_ALLOC_ERR);
    }
    return res;
}
Matrix operator -(const Matrix& arg1, const Matrix& arg2)
{
    return arg1 + (- arg2);
}
Matrix operator *(const Matrix& arg1, const Matrix& arg2)
{
    if(arg1.get_length_y() != arg2.get_length_x()) throw Matrix_ex(M_BAD_SIZES, &arg1, &arg2);
    Matrix mat(arg1.get_length_x(), arg2.get_length_y());
    Rational_number rat;
    try
    {
        for(unsigned int i = 0; i < arg1.get_length_x(); i++)
            for(unsigned int j = 0; j < arg2.get_length_y(); j++)
            {
                rat = 0;
                for(unsigned int k = 0; k < arg1.get_length_y(); k++)
                    rat += arg1[Matrix_coord(i, k)] * arg2[Matrix_coord(k, j)];
                if(rat != 0) mat(i ,j) = rat.make_canonical();
            }

    }
    catch(Rational_number_ex *ex)
    {
        throw ex;
    }
    catch(...)
    {
        throw Matrix_ex(M_ALLOC_ERR);
    }
    return mat;
}
Matrix operator *(const Rational_number& arg1, const Matrix& arg2)
{
    Matrix mat(arg2.get_length_x(), arg2.get_length_y());
    Rational_number rat;
    try
    {
        for(unsigned int i = 0; i < arg2.get_length_x(); i++)
            for(unsigned int j = 0; j < arg2.get_length_y(); j++)
                if((rat = arg1 * mat[i][j]) != 0)
                    mat(i ,j) = rat.make_canonical();
    }
    catch(Rational_number_ex *ex)
    {
        throw ex;
    }
    catch(...)
    {
        throw Matrix_ex(M_ALLOC_ERR);
    }
    return mat;
}
Matrix operator /(const Rational_number& arg1, const Matrix& arg2)
{
    Matrix mat(arg2.get_length_x(), arg2.get_length_y());
    Rational_number rat;
    try
    {
        for(unsigned int i = 0; i < arg2.get_length_x(); i++)
            for(unsigned int j = 0; j < arg2.get_length_y(); j++)
                if((rat = arg1 / mat[i][j]) != 0)
                    mat(i ,j) = rat.make_canonical();
    }
    catch(Rational_number_ex *ex)
    {
        throw ex;
    }
    catch(...)
    {
        throw Matrix_ex(M_ALLOC_ERR);
    }
    return mat;
}
Matrix operator +(const Rational_number& arg1, const Matrix& arg2)
{
    Matrix mat(arg2.get_length_x(), arg2.get_length_y());
    Rational_number rat;
    try
    {
        for(unsigned int i = 0; i < arg2.get_length_x(); i++)
            for(unsigned int j = 0; j < arg2.get_length_y(); j++)
                if((rat = arg1 + mat[i][j]) != 0)
                    mat(i ,j) = rat.make_canonical();
    }
    catch(Rational_number_ex *ex)
    {
        throw ex;
    }
    catch(...)
    {
        throw Matrix_ex(M_ALLOC_ERR);
    }
    return mat;
}
Matrix operator -(const Rational_number& arg1, const Matrix& arg2)
{
    Matrix mat(arg2.get_length_x(), arg2.get_length_y());
    Rational_number rat;
    try
    {
        for(unsigned int i = 0; i < arg2.get_length_x(); i++)
            for(unsigned int j = 0; j < arg2.get_length_y(); j++)
                if((rat = arg1 - mat[i][j]) != 0)
                    mat(i ,j) = rat.make_canonical();
                else
                    mat.remove(mat.tree, Matrix_coord(i, j));
    }
    catch(Rational_number_ex *ex)
    {
        throw ex;
    }
    catch(...)
    {
        throw Matrix_ex(M_ALLOC_ERR);
    }
    return mat;
}
char* to_string(const Matrix& obj)
{
    return obj.get_str();
}
void Matrix::write(const char* file_name) const
{
    ofstream myfile;
    char* s;
    Rational_number rat;
    myfile.open(file_name);
    if(myfile.is_open())
    {
        myfile << "matrix " << length_x << ' ' << length_y << '\n';
        for(unsigned int i = 0; i < length_x; i++)
            for(unsigned int j = 0; j < length_y; j++)
            {

                if((rat = get_elem(tree, Matrix_coord(i, j))) != 0)
                {
                    s = rat.get_str();
                    myfile << i + 1 << ' ' << j + 1 << ' ' << s << '\n';
                    free(s);
                }
            }
    }
    else throw Matrix_ex(M_UNABLE_TO_OPEN,0,0,file_name);
}
//tree functions
unsigned char Matrix::height(mnode* p) const
{
    return p?p->height:0;
}
int Matrix::bfactor(mnode* p) const
{
    return height(p->right)-height(p->left);
}
void Matrix::fixheight(mnode* p)
{
    unsigned char hl = height(p->left);
    unsigned char hr = height(p->right);
    p->height = (hl>hr?hl:hr)+1;
}
mnode* Matrix::rotateright(mnode* p) // правый поворот вокруг p
{
    mnode* q = p->left;
    p->left = q->right;
    q->right = p;
    fixheight(p);
    fixheight(q);
    return q;
}
mnode* Matrix::rotateleft(mnode* q) // левый поворот вокруг q
{
    mnode* p = q->right;
    q->right = p->left;
    p->left = q;
    fixheight(q);
    fixheight(p);
    return p;
}
mnode* Matrix::balance(mnode* p) // балансировка узла p
{
    fixheight(p);
    if( bfactor(p)==2 )
    {
        if( bfactor(p->right) < 0 )
            p->right = rotateright(p->right);
        return rotateleft(p);
    }
    if( bfactor(p)==-2 )
    {
        if( bfactor(p->left) > 0  )
            p->left = rotateleft(p->left);
        return rotateright(p);
    }
    return p; // балансировка не нужна
}
Rational_number Matrix::get_elem(mnode *p, Matrix_coord k) const
{
    if(p == 0) return 0;
    if( k < p->key ) return get_elem(p->left,k);
    else
    {
        if(k > p->key) return get_elem(p->right,k);
        else return p->data;
    }
}
Rational_number& Matrix::get_elemref(mnode *p, Matrix_coord k)
{
    if( k < p->key ) return get_elemref(p->left,k);
    else
    {
        if(k > p->key) return get_elemref(p->right,k);
        else return p->data;
    }
}
mnode* Matrix::insert(mnode* p, Matrix_coord k, Rational_number d) // вставка ключа k в дерево с корнем p
{
    if( p == 0 ) return new mnode(k, d);
    if( k < p->key )
        p->left = insert(p->left,k, d);
    else
    {
        if(k > p->key)p->right = insert(p->right,k, d);
        else p->data = d;
    }
    return balance(p);
}
mnode* Matrix::findmin(mnode* p) // поиск узла с минимальным ключом в дереве p
{
    return p->left?findmin(p->left):p;
}
mnode* Matrix::removemin(mnode* p) // удаление узла с минимальным ключом из дерева p
{
    if( p->left==0 )
        return p->right;
    p->left = removemin(p->left);
    return balance(p);
}
mnode* Matrix::remove(mnode* p, Matrix_coord k) // удаление ключа k из дерева p
{
    if( p == 0 ) return 0;
    if( k < p->key )
        p->left = remove(p->left,k);
    else if( k > p->key )
        p->right = remove(p->right,k);
    else //  k == p->key
    {
        mnode* q = p->left;
        mnode* r = p->right;
        delete p;
        if( !r ) return q;
        mnode* min = findmin(r);
        min->right = removemin(r);
        min->left = q;
        return balance(min);
    }
    return balance(p);
}
mnode* Matrix::remove(mnode *p)
{
    if(p == 0) return 0;
    if(p->right != 0) remove(p->right);
    if(p->left != 0) remove(p->left);
    delete p;
    return 0;
}
std::ostream& operator << (std::ostream& s,const Matrix &m)
{
    char* c = m.get_str();
    s << c;
    free(c);
    return s;
}
