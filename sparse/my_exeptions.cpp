#include "matrix_ex.h"
#include "vector_ex.h"
#include "rational_number_ex.h"
Vector_ex::Vector_ex(unsigned int c, const Vector *vec1, const Vector *vec2, const char *m)
{
    code = c;
    obj1 = vec1;
    obj2 = vec2;
    switch(code)
    {
        case V_ALLOC_ERR: //Alloc err
            break;
        case V_UNABLE_TO_OPEN: //Can not open file
            try {
                    msg ="Can not open file: ";
                    msg = msg + m;
                }
            catch(...) {code = 1;}
            break;
        case V_NO_VECTOR: //no vector
            try {
                    msg ="\"vector\" not found: ";
                    msg = msg + m;
                }
            catch(...) {code = 1;}
            break;
        case V_NO_LENGTH: //Length not found
            try {
                    msg ="Length not found:";
                    msg = msg + m;
                }
            catch(...) {code = 1;}
            break;
        case V_BAD_FORMAT: //Bad format
            try {
                    msg ="Bad file format:";
                    msg = msg + m;
                }
            catch(...) {code = 1;}
            break;
        case V_ZERO_INDEX://0 index
            try {
                    msg ="0 index:";
                    msg = msg + m;
                }
            catch(...) {code = 1;}
            break;
        case V_F_OUT_OF_RANGE://out of range in file
            try {
                    msg ="out of range in file:";
                    msg = msg + m;
                }
            catch(...) {code = 1;}
        break;
        case V_OUT_OF_RANGE://out of range
            try {
                msg ="out of range";
                }
            catch(...) {code = 1;}
        break;
        case V_BAD_SIZES://Bad sizes
        try {
                msg ="Bad sizes";
            }
        catch(...) {code = 1;}
        break;
        default:
        code = 0;
    }
}
const char* Vector_ex::what() const
{
    if(code == V_ALLOC_ERR) return "Alloc err";
    if(code == 0) return "Unknown err";
    return msg._getchararr();
}
Matrix_ex::Matrix_ex(unsigned int c, const Matrix *mat1, const Matrix *mat2, const char *m)
{
    code = c;
    obj1 = mat1;
    obj2 = mat2;
    switch(code)
    {
        case M_ALLOC_ERR: //Alloc err
            break;
        case M_UNABLE_TO_OPEN: //Can not open file
            try {
                    msg ="Unable to open file: ";
                    msg = msg + m;
                }
            catch(...) {code = 1;}
            break;
        case M_NO_MATRIX: //no matrix
            try {
                    msg ="\"matrix\" not found: ";
                    msg = msg + m;
                }
            catch(...) {code = 1;}
            break;
        case M_NO_LENGTH: //Length not found
            try {
                    msg ="Length not found:";
                    msg = msg + m;
                }
            catch(...) {code = 1;}
            break;
        case M_BAD_FORMAT: //Bad format
            try {
                    msg ="Bad file format:";
                    msg = msg + m;
                }
            catch(...) {code = 1;}
            break;
        case M_ZERO_INDEX://0 index
            try {
                    msg ="0 index:";
                    msg = msg + m;
                }
            catch(...) {code = 1;}
            break;
        case M_F_OUT_OF_RANGE://out of range in file
            try {
                    msg ="out of range in file:";
                    msg = msg + m;
                }
            catch(...) {code = 1;}
        break;
        case M_OUT_OF_RANGE://out of range
            try {
                msg ="out of range";
                }
            catch(...) {code = 1;}
        break;
        case M_BAD_SIZES://Bad sizes
        try {
                msg ="Bad sizes";
            }
        catch(...) {code = 1;}
        break;
        default:
        code = 0;
    }
}
const char* Matrix_ex::what() const
{
    if(code == M_ALLOC_ERR) return "Alloc err";
    if(code == 0) return "Unknown err";
    return msg._getchararr();
}
Rational_number_ex::Rational_number_ex(unsigned int c, const Rational_number *rat1, const Rational_number *rat2, const char *m)
{
    code = c;
    obj1 = rat1;
    obj2 = rat2;
    switch(code)
    {
        case 1: //Alloc err
            break;
        case 2: //Zero devision with 2 arg
            try {
                    msg ="Zero division while working with: ";
                    msg = msg + rat1->get_String() + " " + rat2->get_String();
                }
            catch(...) {code = 1;}
            break;
        case 3: //Zero devision with 1 arg
            try {
                    msg ="Zero division ";
                    msg = msg + rat1->get_String();
                }
            catch(...) {code = 1;}
            break;
        case 6: //Zero devision in constructor
            try {msg ="Zero division in constructor";}
            catch(...) {code = 1;}
            break;
        case 4: //Overflow
            try {msg ="Overflow UINT_32";}
            catch(...) {code = 1;}
            break;
        case 5:
            try {
                    msg ="Bad string format:";
                    msg = msg + m;
                }
            catch(...) {code = 1;}
            break;
        default:
        code = 0;
    }
}
const char* Rational_number_ex::what() const
{
    if(code == 1) return "Alloc err";
    if(code == 0) return "Unknown err";
    return msg._getchararr();
}
