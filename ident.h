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
    unsigned int line;
    unsigned int column;
public:
    Ident() {}
    virtual ~Ident() {}
    Ident(const std::string &str, type_of_lex t, unsigned int l, unsigned int c)
    {
        type = t;
        name = str;
        line = l;
        column = c;
    }
    const std::string& get_name() const{ return name; }
    type_of_lex get_type() const{ return type; }
    unsigned int get_line() const { return line ;}
    unsigned int get_column() const { return column ;}
};

class Ident_Int: public Ident
{
    int value;
public:
    Ident_Int(const std::string &str, const int &val = 0, unsigned int l = 0 , unsigned int c = 0)
        : Ident(str, LEX_INTEGER, l, c)
        { value = val; }
    const int& get_value() const { return value; }
    void set_value(const int& val) { value = val; }
    friend std::ostream& operator << (std::ostream& s, const Ident_Int& obj) { return s << obj.get_value(); }
};

class Ident_Float: public Ident
{
    double value;
public:
    Ident_Float(const std::string &str, const double &val = 0, unsigned int l = 0, unsigned int c = 0)
        : Ident(str, LEX_FLOAT, l, c)
        { value = val; }
    const double& get_value() const { return value; }
    void set_value(const double& val) { value = val; }
    friend std::ostream& operator << (std::ostream& s, const Ident_Float& obj) { return s << obj.get_value(); }
};

class Ident_Rational: public Ident
{
    Rational_number value;
public:
    Ident_Rational(const std::string &str, const Rational_number &val = 0, unsigned int l = 0, unsigned int c = 0)
        : Ident(str, LEX_RATIONAL, l, c)
        { value = val; }
    const Rational_number& get_value() const { return value; }
    void set_value(const Rational_number& val) { value = val; }
    friend std::ostream& operator << (std::ostream& s, const Ident_Rational& obj) { return s << obj.get_value(); }
};

class Ident_Vector: public Ident
{
    Vector value;
    bool rotation;
public:
    Ident_Vector(const std::string &str, const Vector &val = Vector(0), bool r = 0, unsigned int l = 0, unsigned int c = 0)
        : Ident(str, LEX_VECTOR, l, c)
        { value = val; rotation = r; }
    const Vector& get_value() const { return value; }
    bool get_rotation() const { return rotation; }
    void set_rotation(bool r) { rotation = r; }
    void set_value(const Vector& val) { value = val; }
    friend std::ostream& operator << (std::ostream& s, const Ident_Vector& obj) { return s << obj.get_value(); }
};

class Ident_Matrix: public Ident
{
    Matrix value;
public:
    Ident_Matrix(const std::string &str, const Matrix &val = Matrix((unsigned int)0), unsigned int l = 0, unsigned int c = 0)
        : Ident(str, LEX_MATRIX, l, c)
        { value = val; }
    const Matrix& get_value() const { return value; }
    void set_value(const Matrix& val) { value = val; }
    friend std::ostream& operator << (std::ostream& s, const Ident_Matrix& obj) { return s << obj.get_value(); }
};
class Ident_String: public Ident
{
    std::string value;
public:
    Ident_String(const std::string &str, const std::string &val = "", unsigned int l = 0, unsigned int c = 0)
        : Ident(str, LEX_STRING, l, c)
        { value = val; }
    const std::string& get_value() const { return value; }
    void set_value(const std::string& val) { value = val; }
    friend std::ostream& operator << (std::ostream& s, const Ident_String& obj) { return s << obj.get_value(); }
};
#endif // IDENT_H
