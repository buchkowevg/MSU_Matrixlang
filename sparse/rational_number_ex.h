#ifndef RATIONAL_NUMBER_EX_H
#define RATIONAL_NUMBER_EX_H
#include "rational_number.h"
#include "sparse_ex.h"
#define R_ALLOC_ERR 1
#define R_ZERO_DIV_1 2
#define R_ZERO_DIV_2 3
#define R_ZERO_DIV_CONSTRUCT 6
#define R_OVERFLOW 4
#define R_BAD_STRING_FORMAT 5
class Rational_number_ex : public Sparse_ex
{
public:
    Rational_number_ex() {}
    Rational_number_ex(unsigned int c, const Rational_number *rat1 = 0, const Rational_number *rat2 = 0, const char* m = "");
    const char * what () const;
    const Rational_number *obj1;
    const Rational_number *obj2;
private:
    String msg;
    unsigned int code;
};

#endif // RATIONAL_NUMBER_EX_H
