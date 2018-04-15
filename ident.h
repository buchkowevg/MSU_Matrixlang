#ifndef IDENT_H
#define IDENT_H
#include <string>
#include <ostream>
#include "lex.h"
#include "../sparse/matrix.h"

class Ident
{
protected:
    std::string name;
    type_of_lex type;
public:
    Ident() {}
    virtual ~Ident() {}
    Ident(const std::string &str, type_of_lex t)
    {
        type = t;
        name = str;
    }
    const std::string& get_name() const{ return name; }
    type_of_lex get_type() const{ return type; }
};

class Ident_Int: public Ident
{
    int value;
public:
    Ident_Int(const std::string &str, const int &val = 0) : Ident(str, LEX_INTEGER)
        { value = val; }
    const int& get_value() const { return value; }
    void set_value(const int& val) { value = val; }
    friend std::ostream& operator << (std::ostream& s, const Ident_Int& obj) { return s << obj.get_value(); }
};

class Ident_Float: public Ident
{
    double value;
public:
    Ident_Float(const std::string &str, const double &val = 0) : Ident(str, LEX_FLOAT)
        { value = val; }
    const double& get_value() const { return value; }
    void set_value(const double& val) { value = val; }
    friend std::ostream& operator << (std::ostream& s, const Ident_Float& obj) { return s << obj.get_value(); }
};

class Ident_Rational: public Ident
{
    Rational_number value;
public:
    Ident_Rational(const std::string &str, const Rational_number &val = 0) : Ident(str, LEX_RATIONAL)
        { value = val; }
    const Rational_number& get_value() const { return value; }
    void set_value(const Rational_number& val) { value = val; }
    friend std::ostream& operator << (std::ostream& s, const Ident_Rational& obj) { return s << obj.get_value(); }
};

class Ident_Vector: public Ident
{
    Vector value;
public:
    Ident_Vector(const std::string &str, const Vector &val = Vector(0)) : Ident(str, LEX_VECTOR)
        { value = val; }
    const Vector& get_value() const { return value; }
    void set_value(const Vector& val) { value = val; }
    friend std::ostream& operator << (std::ostream& s, const Ident_Vector& obj) { return s << obj.get_value(); }
};

class Ident_Matrix: public Ident
{
    Matrix value;
public:
    Ident_Matrix(const std::string &str, const Matrix &val = Matrix((unsigned int)0)) : Ident(str, LEX_MATRIX)
        { value = val; }
    const Matrix& get_value() const { return value; }
    void set_value(const Matrix& val) { value = val; }
    friend std::ostream& operator << (std::ostream& s, const Ident_Matrix& obj) { return s << obj.get_value(); }
};
#endif // IDENT_H
