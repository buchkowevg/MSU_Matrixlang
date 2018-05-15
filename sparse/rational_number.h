#ifndef RATIONAL_NUMBER_H
#define RATIONAL_NUMBER_H
#include <inttypes.h>
#include "mystring.h"
#include <ostream>
#include <stdlib.h> 

class Rational_number
{
public:
    Rational_number();
    Rational_number(const Rational_number &obj);
    Rational_number(uint32_t n, uint32_t d);
    Rational_number(bool s, uint32_t n, uint32_t d);
    Rational_number(const char *s1, const char *s2);
    Rational_number(const char *str);
    Rational_number(short n);
    Rational_number(int n);
    Rational_number(long int n);
    Rational_number(long long int n);
    Rational_number(unsigned short n);
    Rational_number(unsigned int n);
    Rational_number(unsigned long int n);
    Rational_number(unsigned long long int n);
    Rational_number(double n, unsigned int power = 4);
    Rational_number& operator =(const Rational_number& obj);
    Rational_number& operator +=(const Rational_number& obj);
    Rational_number& operator -=(const Rational_number& obj);
    Rational_number& operator *=(const Rational_number& obj);
    Rational_number& operator /=(const Rational_number& obj);
    Rational_number operator ^(int) const;
    Rational_number operator -() const;
    Rational_number &operator ++();
    Rational_number operator ++(int);
    Rational_number &operator --();
    Rational_number operator --(int);
    friend Rational_number operator + (const Rational_number arg1, const Rational_number arg2);
    friend Rational_number operator - (const Rational_number arg1, const Rational_number arg2);
    friend Rational_number operator * (const Rational_number arg1, const Rational_number arg2);
    friend Rational_number operator / (const Rational_number arg1, const Rational_number arg2);
    friend bool operator > (const Rational_number arg1, const Rational_number arg2);
    friend bool operator < (const Rational_number arg1, const Rational_number arg2);
    friend bool operator >= (const Rational_number arg1, const Rational_number arg2);
    friend bool operator <= (const Rational_number arg1, const Rational_number arg2);
    friend bool operator == (const Rational_number arg1, const Rational_number arg2);
    friend bool operator != (const Rational_number arg1, const Rational_number arg2);
    friend std::ostream& operator << (std::ostream&, Rational_number);
    friend char* to_string(const Rational_number obj);
    friend Rational_number floor(const Rational_number obj);
    friend Rational_number round(const Rational_number obj);
    void set();
    void set(long long int n);
    void set(unsigned long long int n);
    void set(const Rational_number &obj);
    void set(uint32_t n, uint32_t d);
    void set(bool s, uint32_t n, uint32_t d);
    void set(const char *s1, const char *s2);
    void set(const char *str);
    uint32_t get_numinator() const;
    uint32_t get_denuminator() const;
    bool get_sign() const;
    char* get_str() const;
    char* get_true_str() const;
    String get_String() const;
    String get_true_String() const;
    void reduce_fraction();
    short get_short() const;
    int get_int() const;
    long int get_lint() const;
    double get_double() const;
    Rational_number get_number_part() const;
    Rational_number get_fraction_part() const;
    Rational_number make_canonical();
private:
    uint32_t numinator;
    uint32_t denuminator;
    bool sign;
};

#endif // RATIONAL_NUMBER_H
