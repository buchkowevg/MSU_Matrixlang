#ifndef VECTOR_EX
#define VECTOR_EX

#define V_ALLOC_ERR 1
#define V_UNABLE_TO_OPEN 2
#define V_NO_VECTOR 3
#define V_NO_LENGTH 6
#define V_BAD_FORMAT 4
#define V_ZERO_INDEX 5
#define V_F_OUT_OF_RANGE 7
#define V_OUT_OF_RANGE 8
#define V_BAD_SIZES 9
#include "vector.h"
#include "sparse_ex.h"
class Vector_ex : public Sparse_ex
{
public:
    Vector_ex() {}
    Vector_ex(unsigned int c, const Vector *vec1 = 0, const Vector *vec2 = 0, const char* m = "");
    const char * what () const;
    const Vector *obj1;
    const Vector *obj2;
private:
    String msg;
    unsigned int code;

};
#endif // VECTOR_EX

