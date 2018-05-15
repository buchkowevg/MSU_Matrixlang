#include "rational_number.h"
#include <iostream>
#include <inttypes.h>
#include <climits>
#include "rational_number_ex.h"
using namespace std;

#define BAD_FORMAT 1
#define OVERFLOW 2
#define ZERO_DEVISION 3
#define BAD_ALLOC 4
Rational_number::Rational_number()
{
    set();
}
Rational_number::Rational_number(const Rational_number &obj)
{
    set(obj);
}
Rational_number::Rational_number(uint32_t n, uint32_t d)
{
    set(n, d);
}
Rational_number::Rational_number(bool s, uint32_t n, uint32_t d)
{
    set(s, n, d);
}
Rational_number::Rational_number(const char* s1,const char* s2)
{
    set(s1, s2);
}
Rational_number::Rational_number(const char* str)
{
    set(str);
}
Rational_number::Rational_number(short n)
{
    set((long long int)n);
}
Rational_number::Rational_number(int n)
{
    set((long long int)n);
}
Rational_number::Rational_number(long n)
{
    set((long long int)n);
}
Rational_number::Rational_number(long long n)
{
    set((long long int)n);
}
Rational_number::Rational_number(unsigned short n)
{
    set((unsigned long long)n);
}
Rational_number::Rational_number(unsigned int n)
{
    set((unsigned long long)n);
}
Rational_number::Rational_number(unsigned long n)
{
    set((unsigned long long)n);
}
Rational_number::Rational_number(unsigned long long n)
{
    set((unsigned long long)n);
}
Rational_number::Rational_number(double n, unsigned int power)
{
    int j = 10;
    for(unsigned int i = 1; i < power; i++)
        j *= j;
    *this = (long long int)(n * j);
    *this /= j;
    make_canonical();
}
void Rational_number::set()
{
    sign = 0;
    numinator = 0;
    denuminator = 1;
}
void Rational_number::set(long long int n)
{
    set();
    if(n < 0)
    {
        sign = 1;
        n = -n;
    }
    if(uint32_t(n) > UINT32_MAX) throw Rational_number_ex(R_OVERFLOW);
    numinator = uint32_t(n);
    if(numinator == 0) sign = 0;
}
void Rational_number::set(unsigned long long int n)
{
    set();
    if(uint32_t(n) > UINT32_MAX) throw Rational_number_ex(R_OVERFLOW);
    numinator = uint32_t(n);
}
void Rational_number::set(uint32_t n, uint32_t d)
{
    if(d == 0)
    {
        set();
        throw Rational_number_ex(R_OVERFLOW);
    }
    sign = 0;
    numinator = n;
    if(numinator == 0)
    {
        denuminator = 1;
        sign = 0;
    }
    else denuminator = d;
}
void Rational_number::set(bool s, uint32_t n, uint32_t d)
{
    if(d == 0)
    {
     set();
     throw Rational_number_ex(R_OVERFLOW);
    }
    sign = s;
    numinator = n;
    if(numinator == 0)
    {
        denuminator = 1;
        sign = 0;
    }
    else denuminator = d;
}
void Rational_number::set(const Rational_number &obj)
{
    this->numinator = obj.numinator;
    this->denuminator = obj.denuminator;
    this->sign=obj.sign;
}

void Rational_number::set(const char* s1, const char* s2)
{
    unsigned int count = 0;
    String str1(s1);
    String str2(s2);
    try
    {
        if(str1.getlength() < 1) throw Rational_number_ex(R_BAD_STRING_FORMAT,0,0,str1._getchararr());
        if(str1[0] == '-')
        {
            sign = 1;
            if(str1.getlength() < 2) throw Rational_number_ex(R_BAD_STRING_FORMAT,0,0,str1._getchararr());
        }
        else sign = 0;
        count = sign;
        numinator = 0;
        while (count < str1.getlength() && (str1[count] >= '0' && str1[count] <= '9'))
        {
            numinator = numinator * 10 + (str1[count] - '0');
            count++;
        }
        if(count != str1.getlength()) throw Rational_number_ex(R_BAD_STRING_FORMAT,0,0,str1._getchararr());
        if(str2.getlength() == 0)
        {
            denuminator = 1;
            return;
        }
        count = 0;
        if(str2[0] == '-')
        {
            if(str2.getlength() < 2) throw Rational_number_ex(R_BAD_STRING_FORMAT,0,0,str2._getchararr());
            if(sign == 1) sign = 0;
            else sign = 1;
            count = 1;
        }
        denuminator = 0;
        while (count < str2.getlength() && (str2[count] >= '0' && str2[count] <= '9'))
        {
            denuminator = denuminator * 10 + (str2[count] - '0');
            count++;
        }
        if(count != str2.getlength()) throw Rational_number_ex(R_BAD_STRING_FORMAT,0,0,str2._getchararr());
        if(denuminator == 0) throw Rational_number_ex(R_ZERO_DIV_CONSTRUCT);

    }
    catch(const char*)
    {
        set();
        Rational_number_ex(R_ALLOC_ERR);
    }
    if(numinator == 0)
    {
        denuminator = 1;
        sign = 0;
    }
}

void Rational_number::set(const char* str)
{
    unsigned int count = 0;
    String s(str);
    try
    {
        if(s.getlength() < 1) Rational_number_ex(R_BAD_STRING_FORMAT,0,0,s._getchararr());
        if(s[0] == '-') sign = 1;
        else sign = 0;
        if(sign == 1 && s.getlength() < 2) Rational_number_ex(R_BAD_STRING_FORMAT,0,0,s._getchararr());
        count = sign;
        numinator = 0;
        while (count < s.getlength() && s[count] != '/' && (s[count] >= '0' && s[count] <= '9') && s[count] != ' ')
        {
            numinator = numinator * 10 + (s[count] - '0');
            count++;
        }
        if(count == s.getlength())
        {
            denuminator = 1;
            return;
        }
        while(count < s.getlength() && s[count] == ' ') count++;
        if(count == s.getlength())
        {
            denuminator = 1;
            return;
        }
        if(s[count] != '/') throw Rational_number_ex(R_BAD_STRING_FORMAT,0,0,s._getchararr());
        count++;
        if(s[count] == '-')
        {
            sign = !sign;
            count++;
        }
        while(s[count] == ' ' && count < s.getlength()) count++;
        if(count == s.getlength()) throw Rational_number_ex(R_BAD_STRING_FORMAT,0,0,s._getchararr());
        denuminator = 0;
        while(count < s.getlength() && (s[count] >= '0' && s[count] <= '9'))
        {
            denuminator = denuminator * 10 + (str[count] -'0');
            count++;
        }
        while(count < s.getlength() && s[count] == ' ') count++;
        if(count != s.getlength()) throw Rational_number_ex(R_BAD_STRING_FORMAT,0,0,s._getchararr());
        if(denuminator == 0) throw Rational_number_ex(R_ZERO_DIV_CONSTRUCT);
    }
    catch(const char*)
    {
       set();
       throw Rational_number_ex(R_ALLOC_ERR);
    }
    if(numinator == 0)
    {
        denuminator = 1;
        sign = 0;
    }

}
char* Rational_number::get_str() const
{
    String str1;
    String str2;
    int count = 0;
    try
    {
    if(sign == 1)
    {
        str1.setstring("-");
        count++;
    }
    str2.setstring((long long int)numinator);
    str1 = str1 + str2;
    if(denuminator != 1)
    {
        str2.setstring("/");
        str1 = str1 + str2;
        str2.setstring((long long int)denuminator);
        str1 = str1 + str2;
    }
    return str1.getchararr();
    }
    catch(...)
    {
        throw Rational_number_ex(R_ALLOC_ERR);
        return NULL;
    }

}
String Rational_number::get_String() const
{
    String str1;
    String str2;
    int count = 0;
    try
    {
    if(sign == 1)
    {
        str1.setstring("-");
        count++;
    }
    str2.setstring((long long int)numinator);
    str1 = str1 + str2;
    if(denuminator != 1)
    {
        str2.setstring("/");
        str1 = str1 + str2;
        str2.setstring((long long int)denuminator);
        str1 = str1 + str2;
    }
    }
    catch(...)
    {
        throw Rational_number_ex(R_ALLOC_ERR);
    }
    return str1;

}
char* Rational_number::get_true_str() const
{
    String str;
    String str1;
    try
    {
    if(sign == 1) str.setstring("-");
    str1.setstring((long long int)numinator);
    str = str + str1;
    str1.setstring("/");
    str = str + str1;
    str1.setstring((long long int)denuminator);
    str = str + str1;
    }
    catch(...)
    {
        throw Rational_number_ex(R_ALLOC_ERR);
    }
    return str.getchararr();
}
String Rational_number::get_true_String() const
{
    String str;
    String str1;
    try
    {
    if(sign == 1) str.setstring("-");
    str1.setstring((long long int)numinator);
    str = str + str1;
    str1.setstring("/");
    str = str + str1;
    str1.setstring((long long int)denuminator);
    str = str + str1;
    }
    catch(...)
    {
        throw Rational_number_ex(R_ALLOC_ERR);
    }
    return str;
}
uint32_t Rational_number::get_numinator() const
{
    return numinator;
}
uint32_t Rational_number::get_denuminator() const
{
    return denuminator;
}
bool Rational_number::get_sign() const
{
    return sign;
}
short Rational_number::get_short() const
{
    long long s;
    s =numinator/denuminator;
    short a;
    if(s > SHRT_MAX) throw Rational_number_ex(R_OVERFLOW);
    a = short(s);
    if(sign == 1) a = -a;
    return a;
}
int Rational_number::get_int() const
{
    long long s;
    s =numinator/denuminator;
    int a;
    if(s > INT_MAX) throw Rational_number_ex(R_OVERFLOW);
    a = int(s);
    if(sign == 1) a = -a;
    return a;
}
long int Rational_number::get_lint() const
{
    long long s;
    s =numinator/denuminator;
    long int a;
    if(s > LONG_MAX) throw Rational_number_ex(R_OVERFLOW);
    a = (long int)s;
    if(sign == 1) a = -a;
    return a;
}
double Rational_number::get_double() const
{
    return (1 - 2 * sign) * double(numinator)/denuminator;
}
Rational_number Rational_number::get_number_part() const
{
    long long int a = numinator;
    long long b = denuminator;
    uint32_t res = 0;
    while(a >= b)
    {
        res++;
        a = a - b;
    }
    return Rational_number(res, 1);
}
Rational_number Rational_number::get_fraction_part() const
{
    long long int a = numinator;
    long long b = denuminator;
    uint32_t res = 0;
    while(a >= b)
    {
        res++;
        a = a - b;
    }
    return Rational_number(numinator - res * denuminator, denuminator);
}
Rational_number& Rational_number::operator =(const Rational_number& obj)
{
    sign = obj.sign;
    numinator = obj.numinator;
    denuminator = obj.denuminator;
    return *this;
}
Rational_number operator + (const Rational_number arg1, const Rational_number arg2)
{
    Rational_number res;
    unsigned long long int sv, sv2;
    if(arg1.get_sign() == arg2.get_sign())
    {
        if(arg1.get_denuminator() == arg2.get_denuminator())
        {
            sv = arg1.get_numinator();
            sv2 = arg2.get_numinator();
            if((sv = sv + sv2) > UINT32_MAX) throw Rational_number_ex(R_OVERFLOW);
            res.numinator = sv;
            res.denuminator = arg1.get_denuminator();
        }
        else
        {
            if((sv = arg2.get_denuminator() * arg1.get_numinator()) > UINT32_MAX) throw Rational_number_ex(R_OVERFLOW);
            if((sv2 = arg1.get_denuminator() * arg2.get_numinator()) > UINT32_MAX) throw Rational_number_ex(R_OVERFLOW);
            if((sv = sv + sv2) > UINT32_MAX) throw Rational_number_ex(R_OVERFLOW);
            res.numinator = sv;
            if((sv= arg2.get_denuminator() * arg1.get_denuminator()) > UINT32_MAX) throw Rational_number_ex(R_OVERFLOW);
            res.denuminator = sv;
        }
        res.sign = arg2.get_sign();
        if(res.numinator == 0)
        {
            res.denuminator = 1;
            res.sign = 0;
        }
        return res;
    }
    else
    {
        if(arg1.get_sign() == 1)
        {
            return arg2 - Rational_number(0, arg1.get_numinator(), arg1.get_denuminator());
        }
        else
        {
            return arg1 - Rational_number(0, arg2.get_numinator(), arg2.get_denuminator());
        }

    }
}
Rational_number operator - (const Rational_number arg1, const Rational_number arg2)
{
    Rational_number res;
    long long int sv, sv2;
    if(arg1.get_sign() != arg2.get_sign())
    {

        if(arg1.get_sign() == 1)
        {
            return arg1 + Rational_number(1, arg2.get_numinator(), arg2.get_denuminator());
        }
        else
        {
            return arg2 + Rational_number(1, arg1.get_numinator(), arg1.get_denuminator());
        }
    }
    else
    {
        if(arg1.get_denuminator() == arg2.get_denuminator())
        {
            sv = arg1.get_numinator();
            sv2 = arg2.get_numinator();
            if((sv = sv - sv2) < 0)
            {
                res.sign = !arg1.get_sign();
                sv = -sv;
            }
            else res.sign = arg1.get_sign();
            res.numinator = sv;
            res.denuminator = arg1.get_denuminator();
        }
        else
        {

            if((sv = arg2.get_denuminator() * arg1.get_numinator()) > UINT32_MAX) throw Rational_number_ex(R_OVERFLOW);
            if((sv2 = arg1.get_denuminator() * arg2.get_numinator()) > UINT32_MAX) throw Rational_number_ex(R_OVERFLOW);
            if((sv = sv - sv2) < 0)
            {
                res.sign = !arg1.get_sign();
                sv = -sv;
            }
            else res.sign = arg1.get_sign();
            res.numinator = sv;
            if((sv = arg2.get_denuminator() * arg1.get_denuminator()) > UINT32_MAX) throw Rational_number_ex(R_OVERFLOW);
            res.denuminator = sv;
        }
    }
    if(res.numinator == 0)
    {
        res.denuminator = 1;
        res.sign = 0;
    }
    return res;
}
Rational_number Rational_number::operator -() const
{
    Rational_number res = *this;
    res.sign = !res.sign;
    if(res.numinator == 0)
    {
        res.denuminator = 1;
        res.sign = 0;
    }
    return res;
}
Rational_number& Rational_number::operator ++()
{
    *this = *this + 1;
    if(this->numinator == 0)
    {
        this->denuminator = 1;
        this->sign = 0;
    }
    return *this;
}
Rational_number Rational_number::operator ++(int)
{
    Rational_number tmp(*this);
    ++(*this);
    return tmp;
}
Rational_number& Rational_number::operator --()
{
    *this = *this - 1;
    if(this->numinator == 0)
    {
        this->denuminator = 1;
        this->sign = 0;
    }
    return *this;
}
Rational_number Rational_number::operator --(int)
{
    Rational_number tmp(*this);
    --(*this);
    return tmp;
}
Rational_number& Rational_number::operator +=(const Rational_number& obj)
{
    *this = *this + obj;
    if(this->numinator == 0)
    {
        this->denuminator = 1;
        this->sign = 0;
    }
    return *this;
}
Rational_number& Rational_number::operator -=(const Rational_number& obj)
{
    *this = *this - obj;
    if(this->numinator == 0)
    {
        this->denuminator = 1;
        this->sign = 0;
    }
    return *this;
}
Rational_number operator * (const Rational_number arg1, const Rational_number arg2)
{
    Rational_number res;
    unsigned long long int sv;
    if(arg1.get_sign() != arg2.get_sign()) res.sign = 1;
    else res.sign = 0;
    if((sv = arg1.get_numinator() * arg2.get_numinator()) > UINT32_MAX) throw Rational_number_ex(R_OVERFLOW);
    res.numinator = sv;
    if((sv = arg1.get_denuminator() * arg2.get_denuminator()) > UINT32_MAX) throw Rational_number_ex(R_OVERFLOW);
    res.denuminator = sv;
    if(res.numinator == 0)
    {
        res.denuminator = 1;
        res.sign = 0;
    }
    return res;
}
Rational_number operator / (const Rational_number arg1, const Rational_number arg2)
{
    Rational_number res;
    if(arg2.get_numinator() == 0) throw Rational_number_ex(R_ZERO_DIV_1, &arg1, &arg2);
    res = arg1 * Rational_number(arg2.get_sign(), arg2.get_denuminator(), arg2.get_numinator());
    return res;
}
Rational_number& Rational_number::operator *=(const Rational_number& obj)
{
    *this = *this * obj;
    return *this;
}
Rational_number& Rational_number::operator /=(const Rational_number& obj)
{
    *this = *this / obj;
    return *this;
}
bool operator == (const Rational_number a1, const Rational_number a2)
{
    Rational_number arg1 = a1;
    Rational_number arg2 = a2;
    arg1.make_canonical();
    arg2.make_canonical();
    if(arg1.get_sign() != arg2.get_sign()) return 0;
    if(arg1.get_numinator() != arg2.get_numinator()) return 0;
    if(arg1.get_denuminator() != arg2.get_denuminator()) return 0;
    return 1;
}
bool operator != (const Rational_number arg1, const Rational_number arg2)
{
    return !(arg1 == arg2);
}
bool operator > (const Rational_number a1, const Rational_number a2)
{
    Rational_number arg1 = a1;
    Rational_number arg2 = a2;
    arg1.make_canonical();
    arg2.make_canonical();
    if(arg1.get_sign() != arg2.get_sign())
    {
        if(arg1.get_sign() == 1) return 1;
        else return 0;
    }
    if(arg1 == arg2) return 1;
    if(arg1.get_sign() == 0)
    {
        if((arg1 - arg2).get_sign() == 0) return 0;
        else return 1;
    }
    else
    {
        if((arg2 - arg1).get_sign() == 0) return 0;
        else return 1;
    }
}
bool operator < (const Rational_number arg1, const Rational_number arg2)
{
    if(arg1 == arg2) return 0;
    return arg2 > arg1;
}
bool operator <= (const Rational_number arg1, const Rational_number arg2)
{
    if(arg2 == arg1) return 1;
    return arg2 > arg1;
}
bool operator >= (const Rational_number arg1, const Rational_number arg2)
{
    if(arg2 == arg1) return 1;
    return arg1 >arg2;
}
char* to_string(const Rational_number obj)
{
    char* s = NULL;
    s = obj.get_str();
    return s;
}

Rational_number Rational_number::make_canonical()
{
    unsigned long long int a = this->numinator;
    unsigned long long int b = this->denuminator;
    while(a != 0 && b != 0)
    {
        if(a > b) a = a % b;
        else b = b % a;
    }
    if((a = a + b) == 0) return *this;
    this->numinator = this->numinator / a;
    this->denuminator = this->denuminator / a;
    if(this->numinator == 0) sign = 0;
    return *this;
}
Rational_number floor(const Rational_number obj)
{
    if(obj.get_fraction_part() == 0)
        return Rational_number(obj.get_sign(), obj.get_numinator(), obj.get_denuminator()).make_canonical();
    if(obj.get_sign() == 1) return -(obj.get_number_part() + 1);
    else return obj.get_number_part();
}
Rational_number round(const Rational_number obj)
{
    Rational_number a = obj.get_fraction_part();
    if(a.get_numinator() > (a.get_denuminator()/2)) return (1 - 2 * obj.get_sign()) * (obj.get_number_part() + 1);
    else return (1 - 2 * obj.get_sign()) * obj.get_number_part();
}
Rational_number Rational_number::operator ^(int power) const
{
    Rational_number res = 1;
    Rational_number obj = *this;
    unsigned int p;
    if(power < 0)
    {
        obj = 1 / obj;
        power = -power;
    }
    p = power;
    for(unsigned int i = 0; i < p; i++)
    {
        res *= obj;
        res.make_canonical();
    }
    return res;
}
std::ostream& operator <<(std::ostream& s, Rational_number rat)
{
    char *c = rat.get_str();
    s << c;
    free(c);
    return s;
}
