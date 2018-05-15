#ifndef MATRIX_COORD
#define MATRIX_COORD
class Matrix_coord
{
public:
    Matrix_coord() {x = 0; y = 0;}
    Matrix_coord(unsigned int a, unsigned int b) { x = a; y = b; }
    Matrix_coord set(unsigned int a, unsigned int b) { x = a; y = b; return *this; }
    Matrix_coord& operator =(const Matrix_coord& obj)
    {
        x = obj.x;
        y = obj.y;
        return *this;
    }
    unsigned int get_x(){ return x; }
    unsigned int get_y(){ return y; }
    friend bool operator > (const Matrix_coord arg1, const Matrix_coord arg2)
    {
        if(arg1.x > arg2.x) return 1;
        else
        {
            if(arg1.x == arg2.x)
            {
                if(arg1.y > arg2.y) return 1;
                else return 0;
            }
            else return 0;
        }
    }
    friend bool operator < (const Matrix_coord arg1, const Matrix_coord arg2)
    {
        if(arg1.x == arg2.x && arg1.y == arg2.y) return 0;
        else return !(arg1 > arg2);
    }

private:
    unsigned int x;
    unsigned int y;
};
#endif // MATRIX_COORD
