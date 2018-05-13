#include "poliz.h"
#include <iostream>
#include <cmath>
#include <fstream>

using namespace std;
std::string lex_to_string(type_of_lex);
Poliz::Poliz()
{
    type = LEX_NULL;
    lexem = Lex();
}
Poliz::Poliz(type_of_lex t)
{
    type = t;
    lexem = Lex();
}
Poliz::Poliz(type_of_lex t, Lex l)
{
    type = t;
    lexem = l;
}
Poliz::~Poliz() {}
type_of_lex Poliz::get_type() const
{
    return type;
}
void Poliz::set_type(type_of_lex t)
{
    type = t;
}
Lex Poliz::get_lexem() const
{
    return lexem;
}
void Poliz::set_lexem(Lex &l)
{
    lexem = l;
}
Poliz_Const::Poliz_Const() : Poliz(LEX_CONST)
{
    value = nullptr;
}
Poliz_Const::~Poliz_Const()
{
    delete value;
}
Poliz_Const::Poliz_Const(Ident *id, Lex l, type_of_lex t) : Poliz(t, l)
{
    value = id;
}
Ident* Poliz_Const::get_value() const
{
    return value;
}
void Poliz_Const::set_value(Ident *id)
{
    value = id;
}
Poliz_Var::Poliz_Var() : Poliz(LEX_VAR)
{
    value = nullptr;
}
Poliz_Var::Poliz_Var(Ident *id, Lex l) : Poliz(LEX_VAR, l)
{
    value = id;
}
Ident* Poliz_Var::get_value() const
{
    return value;
}
void Poliz_Var::set_value(Ident *id)
{
    value = id;
}
Poliz_Function::Poliz_Function() : Poliz(LEX_FUNCTION)
{
    oper = LEX_NULL;
}
Poliz_Function::Poliz_Function(type_of_lex t, Lex l) : Poliz(LEX_FUNCTION, l)
{
    oper = t;
}
type_of_lex Poliz_Function::get_oper() const
{
    return oper;
}
Poliz_Info::Poliz_Info() : Poliz(LEX_INFO)
{
    str = "";
}
Poliz_Info::Poliz_Info(const std::string &s) : Poliz(LEX_INFO)
{
    str = s;
}
const std::string& Poliz_Info::get_str() const
{
    return str;
}
void Poliz_Info::set_string(std::string & s)
{
    str = s;
}

void Poliz_Function::add(std::stack<Poliz *> & pol)
{
    Poliz *p1 = pol.top();
    Ident *op2 = pol.top()->get_value();
    pol.pop();
    Poliz *p2 = pol.top();
    Ident *op1 = pol.top()->get_value();
    pol.pop();
    switch (op1->get_type()) {
    case LEX_INTEGER:
        if(op2->get_type() == LEX_VECTOR)
            pol.push(new Poliz_Const
                     (new Ident_Vector
                      ("", Rational_number(static_cast<Ident_Int*>(op1)->get_value()) +
                       static_cast<Ident_Vector*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else if(op2->get_type() == LEX_RATIONAL)
            pol.push(new Poliz_Const
                     (new Ident_Rational
                      ("", static_cast<Ident_Int*>(op1)->get_value() +
                       static_cast<Ident_Rational*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else if(op2->get_type() == LEX_FLOAT)
            pol.push(new Poliz_Const
                     (new Ident_Float
                      ("", static_cast<Ident_Int*>(op1)->get_value() +
                       static_cast<Ident_Float*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else if(op2->get_type() == LEX_INTEGER)
            pol.push(new Poliz_Const
                     (new Ident_Int
                      ("", static_cast<Ident_Int*>(op1)->get_value() +
                       static_cast<Ident_Int*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else if(op2->get_type() == LEX_MATRIX)
            pol.push(new Poliz_Const
                     (new Ident_Matrix
                      ("", Rational_number(static_cast<Ident_Int*>(op1)->get_value()) +
                       static_cast<Ident_Matrix*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else
        {

            if(p1->get_type() == LEX_TEMP_CONST)
                delete p1;
            if(p2->get_type() == LEX_TEMP_CONST)
                delete p2;
            throw "no such operand for " + lex_to_string(op1->get_type()) + " " + lex_to_string(op1->get_type());
        }
        break;
    case LEX_FLOAT:
        if(op2->get_type() == LEX_VECTOR)
            pol.push(new Poliz_Const
                     (new Ident_Vector
                      ("", Rational_number(static_cast<Ident_Float*>(op1)->get_value()) +
                       static_cast<Ident_Vector*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else if(op2->get_type() == LEX_RATIONAL)
            pol.push(new Poliz_Const
                     (new Ident_Rational
                      ("", Rational_number(static_cast<Ident_Float*>(op1)->get_value()) +
                       static_cast<Ident_Rational*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else if(op2->get_type() == LEX_INTEGER)
            pol.push(new Poliz_Const
                     (new Ident_Float
                      ("", static_cast<Ident_Float*>(op1)->get_value() +
                       static_cast<Ident_Int*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else if(op2->get_type() == LEX_FLOAT)
            pol.push(new Poliz_Const
                     (new Ident_Float
                      ("", static_cast<Ident_Float*>(op1)->get_value() +
                       static_cast<Ident_Float*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else if(op2->get_type() == LEX_MATRIX)
            pol.push(new Poliz_Const
                     (new Ident_Matrix
                      ("", Rational_number(static_cast<Ident_Float*>(op1)->get_value()) +
                       static_cast<Ident_Matrix*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else
        {
            if(p1->get_type() == LEX_TEMP_CONST)
                delete p1;
            if(p2->get_type() == LEX_TEMP_CONST)
                delete p2;
            throw "no such operand for " + lex_to_string(op1->get_type()) + " " + lex_to_string(op2->get_type());
        }
        break;
    case LEX_RATIONAL:
        if(op2->get_type() == LEX_VECTOR)
            pol.push(new Poliz_Const
                     (new Ident_Vector
                      ("", static_cast<Ident_Rational*>(op1)->get_value() +
                       static_cast<Ident_Vector*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else if(op2->get_type() == LEX_RATIONAL)
            pol.push(new Poliz_Const
                     (new Ident_Rational
                      ("", static_cast<Ident_Rational*>(op1)->get_value() +
                       static_cast<Ident_Rational*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else if(op2->get_type() == LEX_INTEGER)
            pol.push(new Poliz_Const
                     (new Ident_Rational
                      ("", static_cast<Ident_Rational*>(op1)->get_value() +
                       static_cast<Ident_Int*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else if(op2->get_type() == LEX_FLOAT)
            pol.push(new Poliz_Const
                     (new Ident_Rational
                      ("", static_cast<Ident_Rational*>(op1)->get_value() +
                       Rational_number(static_cast<Ident_Float*>(op2)->get_value())), Lex(), LEX_TEMP_CONST));
        else if(op2->get_type() == LEX_MATRIX)
            pol.push(new Poliz_Const
                     (new Ident_Matrix
                      ("", static_cast<Ident_Rational*>(op1)->get_value() +
                       static_cast<Ident_Matrix*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else
        {
            if(p1->get_type() == LEX_TEMP_CONST)
                delete p1;
            if(p2->get_type() == LEX_TEMP_CONST)
                delete p2;
            throw "no such operand for " + lex_to_string(op1->get_type()) + " " + lex_to_string(op2->get_type());
        }
        break;
    case LEX_VECTOR:
        if(op2->get_type() == LEX_VECTOR)
            pol.push(new Poliz_Const
                     (new Ident_Vector
                      ("", static_cast<Ident_Vector*>(op1)->get_value() +
                       static_cast<Ident_Vector*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else
        {
            if(p1->get_type() == LEX_TEMP_CONST)
                delete p1;
            if(p2->get_type() == LEX_TEMP_CONST)
                delete p2;
            throw "no such operand for " + lex_to_string(op1->get_type()) + " " + lex_to_string(op2->get_type());
        }
        break;
    case LEX_MATRIX:
        if(op2->get_type() == LEX_MATRIX)
            pol.push(new Poliz_Const
                     (new Ident_Matrix
                      ("", static_cast<Ident_Matrix*>(op1)->get_value() +
                       static_cast<Ident_Matrix*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else
        {
            if(p1->get_type() == LEX_TEMP_CONST)
                delete p1;
            if(p2->get_type() == LEX_TEMP_CONST)
                delete p2;
            throw "no such operand for " + lex_to_string(op1->get_type()) + " " + lex_to_string(op2->get_type());
        }
        break;
    default:
        if(p1->get_type() == LEX_TEMP_CONST)
            delete p1;
        if(p2->get_type() == LEX_TEMP_CONST)
            delete p2;
        throw "no such operand for " + lex_to_string(op1->get_type()) + " " + lex_to_string(op2->get_type());
        break;
    }
    if(p1->get_type() == LEX_TEMP_CONST)
        delete p1;
    if(p2->get_type() == LEX_TEMP_CONST)
        delete p2;
}
void Poliz_Function::sub(stack<Poliz*>& pol)
{
    Poliz *p1 = pol.top();
    Ident *op2 = pol.top()->get_value();
    pol.pop();
    Poliz *p2 = pol.top();
    Ident *op1 = pol.top()->get_value();
    pol.pop();
    switch (op1->get_type()) {
    case LEX_INTEGER:
        if(op2->get_type() == LEX_VECTOR)
            pol.push(new Poliz_Const
                     (new Ident_Vector
                      ("", Rational_number(static_cast<Ident_Int*>(op1)->get_value()) -
                       static_cast<Ident_Vector*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else if(op2->get_type() == LEX_RATIONAL)
            pol.push(new Poliz_Const
                     (new Ident_Rational
                      ("", static_cast<Ident_Int*>(op1)->get_value() -
                       static_cast<Ident_Rational*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else if(op2->get_type() == LEX_FLOAT)
            pol.push(new Poliz_Const
                     (new Ident_Float
                      ("", static_cast<Ident_Int*>(op1)->get_value() -
                       static_cast<Ident_Float*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else if(op2->get_type() == LEX_INTEGER)
            pol.push(new Poliz_Const
                     (new Ident_Int
                      ("", static_cast<Ident_Int*>(op1)->get_value() -
                       static_cast<Ident_Int*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else if(op2->get_type() == LEX_MATRIX)
            pol.push(new Poliz_Const
                     (new Ident_Matrix
                      ("", Rational_number(static_cast<Ident_Int*>(op1)->get_value()) -
                       static_cast<Ident_Matrix*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else
        {
            if(p1->get_type() == LEX_TEMP_CONST)
                delete p1;
            if(p2->get_type() == LEX_TEMP_CONST)
                delete p2;
            throw "no such operand for " + lex_to_string(op1->get_type()) + " " + lex_to_string(op2->get_type());
        }
        break;
    case LEX_FLOAT:
        if(op2->get_type() == LEX_VECTOR)
            pol.push(new Poliz_Const
                     (new Ident_Vector
                      ("", Rational_number(static_cast<Ident_Float*>(op1)->get_value()) -
                       static_cast<Ident_Vector*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else if(op2->get_type() == LEX_RATIONAL)
            pol.push(new Poliz_Const
                     (new Ident_Rational
                      ("", Rational_number(static_cast<Ident_Float*>(op1)->get_value()) -
                       static_cast<Ident_Rational*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else if(op2->get_type() == LEX_INTEGER)
            pol.push(new Poliz_Const
                     (new Ident_Float
                      ("", static_cast<Ident_Float*>(op1)->get_value() -
                       static_cast<Ident_Int*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else if(op2->get_type() == LEX_FLOAT)
            pol.push(new Poliz_Const
                     (new Ident_Float
                      ("", static_cast<Ident_Float*>(op1)->get_value() -
                       static_cast<Ident_Float*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else if(op2->get_type() == LEX_MATRIX)
            pol.push(new Poliz_Const
                     (new Ident_Matrix
                      ("", Rational_number(static_cast<Ident_Float*>(op1)->get_value()) -
                       static_cast<Ident_Matrix*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else
        {
            if(p1->get_type() == LEX_TEMP_CONST)
                delete p1;
            if(p2->get_type() == LEX_TEMP_CONST)
                delete p2;
            throw "no such operand for " + lex_to_string(op1->get_type()) + " " + lex_to_string(op2->get_type());
        }
        break;
    case LEX_RATIONAL:
        if(op2->get_type() == LEX_VECTOR)
            pol.push(new Poliz_Const
                     (new Ident_Vector
                      ("", static_cast<Ident_Rational*>(op1)->get_value() -
                       static_cast<Ident_Vector*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else if(op2->get_type() == LEX_RATIONAL)
            pol.push(new Poliz_Const
                     (new Ident_Rational
                      ("", static_cast<Ident_Rational*>(op1)->get_value() -
                       static_cast<Ident_Rational*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else if(op2->get_type() == LEX_INTEGER)
            pol.push(new Poliz_Const
                     (new Ident_Rational
                      ("", static_cast<Ident_Rational*>(op1)->get_value() -
                       static_cast<Ident_Int*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else if(op2->get_type() == LEX_FLOAT)
            pol.push(new Poliz_Const
                     (new Ident_Rational
                      ("", static_cast<Ident_Rational*>(op1)->get_value() -
                       Rational_number(static_cast<Ident_Float*>(op2)->get_value())), Lex(), LEX_TEMP_CONST));
        else if(op2->get_type() == LEX_MATRIX)
            pol.push(new Poliz_Const
                     (new Ident_Matrix
                      ("", static_cast<Ident_Rational*>(op1)->get_value() -
                       static_cast<Ident_Matrix*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else
        {
            if(p1->get_type() == LEX_TEMP_CONST)
                delete p1;
            if(p2->get_type() == LEX_TEMP_CONST)
                delete p2;
            throw "no such operand for " + lex_to_string(op1->get_type()) + " " + lex_to_string(op2->get_type());
        }
        break;
    case LEX_VECTOR:
        if(op2->get_type() == LEX_VECTOR)
            pol.push(new Poliz_Const
                     (new Ident_Vector
                      ("", static_cast<Ident_Vector*>(op1)->get_value() -
                       static_cast<Ident_Vector*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else
        {
            if(p1->get_type() == LEX_TEMP_CONST)
                delete p1;
            if(p2->get_type() == LEX_TEMP_CONST)
                delete p2;
            throw "no such operand for " + lex_to_string(op1->get_type()) + " " + lex_to_string(op2->get_type());
        }
        break;
    case LEX_MATRIX:
        if(op2->get_type() == LEX_MATRIX)
            pol.push(new Poliz_Const
                     (new Ident_Matrix
                      ("", static_cast<Ident_Matrix*>(op1)->get_value() -
                       static_cast<Ident_Matrix*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else
        {
            if(p1->get_type() == LEX_TEMP_CONST)
                delete p1;
            if(p2->get_type() == LEX_TEMP_CONST)
                delete p2;
            throw "no such operand for " + lex_to_string(op1->get_type()) + " " + lex_to_string(op2->get_type());
        }
        break;
    default:
        if(p1->get_type() == LEX_TEMP_CONST)
            delete p1;
        if(p2->get_type() == LEX_TEMP_CONST)
            delete p2;
        throw "no such operand for " + lex_to_string(op1->get_type()) + " " + lex_to_string(op2->get_type());
        break;
    }
    if(p1->get_type() == LEX_TEMP_CONST)
        delete p1;
    if(p2->get_type() == LEX_TEMP_CONST)
        delete p2;
}
void Poliz_Function::mul(std::stack<Poliz *> &pol)
{
    Poliz *p1 = pol.top();
    Ident *op2 = pol.top()->get_value();
    pol.pop();
    Poliz *p2 = pol.top();
    Ident *op1 = pol.top()->get_value();
    pol.pop();
    switch (op1->get_type()) {
    case LEX_INTEGER:
        if(op2->get_type() == LEX_VECTOR)
            pol.push(new Poliz_Const
                     (new Ident_Vector
                      ("", Rational_number(static_cast<Ident_Int*>(op1)->get_value()) *
                       static_cast<Ident_Vector*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else if(op2->get_type() == LEX_RATIONAL)
            pol.push(new Poliz_Const
                     (new Ident_Rational
                      ("", static_cast<Ident_Int*>(op1)->get_value() *
                       static_cast<Ident_Rational*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else if(op2->get_type() == LEX_FLOAT)
            pol.push(new Poliz_Const
                     (new Ident_Float
                      ("", static_cast<Ident_Int*>(op1)->get_value() *
                       static_cast<Ident_Float*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else if(op2->get_type() == LEX_INTEGER)
            pol.push(new Poliz_Const
                     (new Ident_Int
                      ("", static_cast<Ident_Int*>(op1)->get_value() *
                       static_cast<Ident_Int*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else if(op2->get_type() == LEX_MATRIX)
            pol.push(new Poliz_Const
                     (new Ident_Matrix
                      ("", Rational_number(static_cast<Ident_Int*>(op1)->get_value()) *
                       static_cast<Ident_Matrix*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else
        {
            if(p1->get_type() == LEX_TEMP_CONST)
                delete p1;
            if(p2->get_type() == LEX_TEMP_CONST)
                delete p2;
            throw "no such operand for " + lex_to_string(op1->get_type()) + " " + lex_to_string(op2->get_type());
        }
        break;
    case LEX_FLOAT:
        if(op2->get_type() == LEX_VECTOR)
            pol.push(new Poliz_Const
                     (new Ident_Vector
                      ("", Rational_number(static_cast<Ident_Float*>(op1)->get_value()) *
                       static_cast<Ident_Vector*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else if(op2->get_type() == LEX_RATIONAL)
            pol.push(new Poliz_Const
                     (new Ident_Rational
                      ("", Rational_number(static_cast<Ident_Float*>(op1)->get_value()) *
                       static_cast<Ident_Rational*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else if(op2->get_type() == LEX_INTEGER)
            pol.push(new Poliz_Const
                     (new Ident_Float
                      ("", static_cast<Ident_Float*>(op1)->get_value() *
                       static_cast<Ident_Int*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else if(op2->get_type() == LEX_FLOAT)
            pol.push(new Poliz_Const
                     (new Ident_Float
                      ("", static_cast<Ident_Float*>(op1)->get_value() *
                       static_cast<Ident_Float*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else if(op2->get_type() == LEX_MATRIX)
            pol.push(new Poliz_Const
                     (new Ident_Matrix
                      ("", Rational_number(static_cast<Ident_Float*>(op1)->get_value()) *
                       static_cast<Ident_Matrix*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else
        {
            if(p1->get_type() == LEX_TEMP_CONST)
                delete p1;
            if(p2->get_type() == LEX_TEMP_CONST)
                delete p2;
            throw "no such operand for " + lex_to_string(op1->get_type()) + " " + lex_to_string(op2->get_type());
        }
        break;
    case LEX_RATIONAL:
        if(op2->get_type() == LEX_VECTOR)
            pol.push(new Poliz_Const
                     (new Ident_Vector
                      ("", static_cast<Ident_Rational*>(op1)->get_value() *
                       static_cast<Ident_Vector*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else if(op2->get_type() == LEX_RATIONAL)
            pol.push(new Poliz_Const
                     (new Ident_Rational
                      ("", static_cast<Ident_Rational*>(op1)->get_value() *
                       static_cast<Ident_Rational*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else if(op2->get_type() == LEX_INTEGER)
            pol.push(new Poliz_Const
                     (new Ident_Rational
                      ("", static_cast<Ident_Rational*>(op1)->get_value() *
                       static_cast<Ident_Int*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else if(op2->get_type() == LEX_FLOAT)
            pol.push(new Poliz_Const
                     (new Ident_Rational
                      ("", static_cast<Ident_Rational*>(op1)->get_value() *
                       Rational_number(static_cast<Ident_Float*>(op2)->get_value())), Lex(), LEX_TEMP_CONST));
        else if(op2->get_type() == LEX_MATRIX)
            pol.push(new Poliz_Const
                     (new Ident_Matrix
                      ("", static_cast<Ident_Rational*>(op1)->get_value() *
                       static_cast<Ident_Matrix*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else
        {
            if(p1->get_type() == LEX_TEMP_CONST)
                delete p1;
            if(p2->get_type() == LEX_TEMP_CONST)
                delete p2;
            throw "no such operand for " + lex_to_string(op1->get_type()) + " " + lex_to_string(op2->get_type());
        }
        break;
    case LEX_VECTOR:
        if(op2->get_type() == LEX_VECTOR)
        {
            if(static_cast<Ident_Vector*>(op1)->get_rotation() != static_cast<Ident_Vector*>(op2)->get_rotation())
            pol.push(new Poliz_Const
                     (new Ident_Rational
                      ("", static_cast<Ident_Vector*>(op1)->get_value() *
                       static_cast<Ident_Vector*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
            else
            {
                if(p1->get_type() == LEX_TEMP_CONST)
                    delete p1;
                if(p2->get_type() == LEX_TEMP_CONST)
                    delete p2;
                throw string("Bad sizes");
            }
        }
        else if(op2->get_type() == LEX_MATRIX)
        {
            Ident_Vector* v = static_cast<Ident_Vector*>(op1);
            if(v->get_rotation() || v->get_value().get_number_of_elements() < 2)
                pol.push(new Poliz_Const
                         (new Ident_Vector
                          ("", (Matrix(v->get_value(), 1) *
                           static_cast<Ident_Matrix*>(op2)->get_value()).get_row(0), true), Lex(), LEX_TEMP_CONST));
            else
            {
                if(p1->get_type() == LEX_TEMP_CONST)
                    delete p1;
                if(p2->get_type() == LEX_TEMP_CONST)
                    delete p2;
                throw string("bad sizes");
            }
        }
        else
        {
            if(p1->get_type() == LEX_TEMP_CONST)
                delete p1;
            if(p2->get_type() == LEX_TEMP_CONST)
                delete p2;
            throw "no such operand for " + lex_to_string(op1->get_type()) + " " + lex_to_string(op2->get_type());
        }
        break;
    case LEX_MATRIX:
        if(op2->get_type() == LEX_MATRIX)
            pol.push(new Poliz_Const
                     (new Ident_Matrix
                      ("", static_cast<Ident_Matrix*>(op1)->get_value() *
                       static_cast<Ident_Matrix*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else if(op2->get_type() == LEX_VECTOR)
        {
            Ident_Vector* v = static_cast<Ident_Vector*>(op2);
            if(!v->get_rotation() || v->get_value().get_number_of_elements() < 2)
                pol.push(new Poliz_Const
                         (new Ident_Vector
                          ("", static_cast<Ident_Matrix*>(op1)->get_value() *
                           v->get_value()), Lex(), LEX_TEMP_CONST));
            else
            {
                if(p1->get_type() == LEX_TEMP_CONST)
                    delete p1;
                if(p2->get_type() == LEX_TEMP_CONST)
                    delete p2;
                throw string("bad sizes");
            }
        }
        else
        {
            if(p1->get_type() == LEX_TEMP_CONST)
                delete p1;
            if(p2->get_type() == LEX_TEMP_CONST)
                delete p2;
            throw "no such operand for " + lex_to_string(op1->get_type()) + " " + lex_to_string(op2->get_type());
        }
        break;
    default:
        if(p1->get_type() == LEX_TEMP_CONST)
            delete p1;
        if(p2->get_type() == LEX_TEMP_CONST)
            delete p2;
        throw "no such operand for " + lex_to_string(op1->get_type()) + " " + lex_to_string(op2->get_type());
        break;
    }
    if(p1->get_type() == LEX_TEMP_CONST)
        delete p1;
    if(p2->get_type() == LEX_TEMP_CONST)
        delete p2;
}
void Poliz_Function::div(std::stack<Poliz *> &pol)
{
    Poliz *p1 = pol.top();
    Ident *op2 = pol.top()->get_value();
    pol.pop();
    Poliz *p2 = pol.top();
    Ident *op1 = pol.top()->get_value();
    pol.pop();
    switch (op1->get_type()) {
    case LEX_INTEGER:
        if(op2->get_type() == LEX_VECTOR)
            pol.push(new Poliz_Const
                     (new Ident_Vector
                      ("", Rational_number(static_cast<Ident_Int*>(op1)->get_value()) /
                       static_cast<Ident_Vector*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else if(op2->get_type() == LEX_RATIONAL)
            pol.push(new Poliz_Const
                     (new Ident_Rational
                      ("", static_cast<Ident_Int*>(op1)->get_value() /
                       static_cast<Ident_Rational*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else if(op2->get_type() == LEX_FLOAT)
        {
            if(static_cast<Ident_Float*>(op2)->get_value() == 0)
            {
                if(p1->get_type() == LEX_TEMP_CONST)
                    delete p1;
                if(p2->get_type() == LEX_TEMP_CONST)
                    delete p2;
                throw "devide by zero";
            }
            pol.push(new Poliz_Const
                     (new Ident_Float
                      ("", static_cast<Ident_Int*>(op1)->get_value() /
                       static_cast<Ident_Float*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        }
        else if(op2->get_type() == LEX_INTEGER)
        {
            if(static_cast<Ident_Int*>(op2)->get_value() == 0)
            {
                if(p1->get_type() == LEX_TEMP_CONST)
                    delete p1;
                if(p2->get_type() == LEX_TEMP_CONST)
                    delete p2;
                throw "devide by zero";
            }
            pol.push(new Poliz_Const
                     (new Ident_Rational
                      ("", Rational_number(static_cast<Ident_Int*>(op1)->get_value() /
                                           static_cast<Ident_Int*>(op2)->get_value())), Lex(), LEX_TEMP_CONST));
        }
        else if(op2->get_type() == LEX_MATRIX)
            pol.push(new Poliz_Const
                     (new Ident_Matrix
                      ("", Rational_number(static_cast<Ident_Int*>(op1)->get_value()) -
                       static_cast<Ident_Matrix*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else
        {
            if(p1->get_type() == LEX_TEMP_CONST)
                delete p1;
            if(p2->get_type() == LEX_TEMP_CONST)
                delete p2;
            throw "no such operand for " + lex_to_string(op1->get_type()) + " " + lex_to_string(op2->get_type());
        }
        break;
    case LEX_FLOAT:
        if(op2->get_type() == LEX_VECTOR)
            pol.push(new Poliz_Const
                     (new Ident_Vector
                      ("", Rational_number(static_cast<Ident_Float*>(op1)->get_value()) /
                       static_cast<Ident_Vector*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else if(op2->get_type() == LEX_RATIONAL)
            pol.push(new Poliz_Const
                     (new Ident_Rational
                      ("", Rational_number(static_cast<Ident_Float*>(op1)->get_value()) /
                       static_cast<Ident_Rational*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else if(op2->get_type() == LEX_INTEGER)
        {
            if(static_cast<Ident_Int*>(op2)->get_value() == 0)
            {
                if(p1->get_type() == LEX_TEMP_CONST)
                    delete p1;
                if(p2->get_type() == LEX_TEMP_CONST)
                    delete p2;
                throw "devide by zero";
            }
            pol.push(new Poliz_Const
                     (new Ident_Float
                      ("", static_cast<Ident_Float*>(op1)->get_value() /
                       static_cast<Ident_Int*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        }
        else if(op2->get_type() == LEX_FLOAT)
        {
            if(static_cast<Ident_Float*>(op2)->get_value() == 0)
            {
                if(p1->get_type() == LEX_TEMP_CONST)
                    delete p1;
                if(p2->get_type() == LEX_TEMP_CONST)
                    delete p2;
                throw "devide by zero";
            }
            pol.push(new Poliz_Const
                     (new Ident_Float
                      ("", static_cast<Ident_Float*>(op1)->get_value() /
                       static_cast<Ident_Float*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        }
        else if(op2->get_type() == LEX_MATRIX)
            pol.push(new Poliz_Const
                     (new Ident_Matrix
                      ("", Rational_number(static_cast<Ident_Float*>(op1)->get_value()) /
                       static_cast<Ident_Matrix*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else
        {
            if(p1->get_type() == LEX_TEMP_CONST)
                delete p1;
            if(p2->get_type() == LEX_TEMP_CONST)
                delete p2;
            throw "no such operand for " + lex_to_string(op1->get_type()) + " " + lex_to_string(op2->get_type());
        }
        break;
    case LEX_RATIONAL:
        if(op2->get_type() == LEX_VECTOR)
            pol.push(new Poliz_Const
                     (new Ident_Vector
                      ("", static_cast<Ident_Rational*>(op1)->get_value() /
                       static_cast<Ident_Vector*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else if(op2->get_type() == LEX_RATIONAL)
            pol.push(new Poliz_Const
                     (new Ident_Rational
                      ("", static_cast<Ident_Rational*>(op1)->get_value() /
                       static_cast<Ident_Rational*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else if(op2->get_type() == LEX_INTEGER)
        {
            if(static_cast<Ident_Int*>(op2)->get_value() == 0)
            {
                if(p1->get_type() == LEX_TEMP_CONST)
                    delete p1;
                if(p2->get_type() == LEX_TEMP_CONST)
                    delete p2;
                throw "devide by zero";
            }
            pol.push(new Poliz_Const
                     (new Ident_Rational
                      ("", static_cast<Ident_Rational*>(op1)->get_value() /
                       static_cast<Ident_Int*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        }
        else if(op2->get_type() == LEX_FLOAT)
            pol.push(new Poliz_Const
                     (new Ident_Rational
                      ("", static_cast<Ident_Rational*>(op1)->get_value() /
                       Rational_number(static_cast<Ident_Float*>(op2)->get_value())), Lex(), LEX_TEMP_CONST));
        else if(op2->get_type() == LEX_MATRIX)
            pol.push(new Poliz_Const
                     (new Ident_Matrix
                      ("", static_cast<Ident_Rational*>(op1)->get_value() /
                       static_cast<Ident_Matrix*>(op2)->get_value()), Lex(), LEX_TEMP_CONST));
        else
        {
            if(p1->get_type() == LEX_TEMP_CONST)
                delete p1;
            if(p2->get_type() == LEX_TEMP_CONST)
                delete p2;
            throw "no such operand for " + lex_to_string(op1->get_type()) + " " + lex_to_string(op2->get_type());
        }
        break;
    default:
        if(p1->get_type() == LEX_TEMP_CONST)
            delete p1;
        if(p2->get_type() == LEX_TEMP_CONST)
            delete p2;
        throw "no such operand for " + lex_to_string(op1->get_type()) + " " + lex_to_string(op2->get_type());
        break;
    }
    if(p1->get_type() == LEX_TEMP_CONST)
        delete p1;
    if(p2->get_type() == LEX_TEMP_CONST)
        delete p2;
}
void Poliz_Function::equal(std::stack<Poliz *> &pol)
{
    Poliz *p2 = pol.top();
    Ident *op2 = pol.top()->get_value();
    pol.pop();
    Poliz *p1 = pol.top();
    Ident *op1 = pol.top()->get_value();
    pol.pop();
    if(p1->get_type() != LEX_VAR)
    {
        if(p1->get_type() == LEX_TEMP_CONST)
            delete p1;
        if(p2->get_type() == LEX_TEMP_CONST)
            delete p2;
        throw string("left operand must be non const");
    }
    switch (op1->get_type()) {
    case LEX_INTEGER:
        if(op2->get_type() == LEX_RATIONAL)
        {
            static_cast<Ident_Int*>(op1)->set_value
                    (static_cast<Ident_Rational*>(op2)->get_value().get_int());
            pol.push(new Poliz_Const
                     (new Ident_Int
                      ("",static_cast<Ident_Int*>(op1)->get_value()), Lex(), LEX_TEMP_CONST));
        }
        else if(op2->get_type() == LEX_FLOAT)
        {
            static_cast<Ident_Int*>(op1)->set_value
                    (static_cast<Ident_Float*>(op2)->get_value());
            pol.push(new Poliz_Const
                     (new Ident_Int
                      ("",static_cast<Ident_Int*>(op1)->get_value()), Lex(), LEX_TEMP_CONST));
        }
        else if(op2->get_type() == LEX_INTEGER)
        {
            static_cast<Ident_Int*>(op1)->set_value
                    (static_cast<Ident_Int*>(op2)->get_value());
            pol.push(new Poliz_Const
                     (new Ident_Int
                      ("",static_cast<Ident_Int*>(op1)->get_value()), Lex(), LEX_TEMP_CONST));
        }
        else
        {
            if(p1->get_type() == LEX_TEMP_CONST)
                delete p1;
            if(p2->get_type() == LEX_TEMP_CONST)
                delete p2;
            throw "no such operand for " + lex_to_string(op1->get_type()) + " " + lex_to_string(op2->get_type());
        }
        break;
    case LEX_FLOAT:
        if(op2->get_type() == LEX_RATIONAL)
        {
            static_cast<Ident_Float*>(op1)->set_value
                    (static_cast<Ident_Rational*>(op2)->get_value().get_double());
            pol.push(new Poliz_Const
                     (new Ident_Float
                      ("",static_cast<Ident_Float*>(op1)->get_value()), Lex(), LEX_TEMP_CONST));
        }
        else if(op2->get_type() == LEX_INTEGER)
        {
            static_cast<Ident_Float*>(op1)->set_value
                    (static_cast<Ident_Int*>(op2)->get_value());
            pol.push(new Poliz_Const
                     (new Ident_Float
                      ("",static_cast<Ident_Float*>(op1)->get_value()), Lex(), LEX_TEMP_CONST));
        }
        else if(op2->get_type() == LEX_FLOAT)
        {
            static_cast<Ident_Float*>(op1)->set_value
                    (static_cast<Ident_Float*>(op2)->get_value());
            pol.push(new Poliz_Const
                     (new Ident_Float
                      ("",static_cast<Ident_Float*>(op1)->get_value()), Lex(), LEX_TEMP_CONST));
        }
        else
        {
            if(p1->get_type() == LEX_TEMP_CONST)
                delete p1;
            if(p2->get_type() == LEX_TEMP_CONST)
                delete p2;
            throw "no such operand for " + lex_to_string(op1->get_type()) + " " + lex_to_string(op2->get_type());
        }
        break;
    case LEX_RATIONAL:
        if(op2->get_type() == LEX_RATIONAL)
        {
            static_cast<Ident_Rational*>(op1)->set_value
                    (static_cast<Ident_Rational*>(op2)->get_value());
            pol.push(new Poliz_Const
                     (new Ident_Rational
                      ("",static_cast<Ident_Rational*>(op1)->get_value()), Lex(), LEX_TEMP_CONST));
        }
        else if(op2->get_type() == LEX_INTEGER)
        {
            static_cast<Ident_Rational*>(op1)->set_value
                    (static_cast<Ident_Int*>(op2)->get_value());
            pol.push(new Poliz_Const
                     (new Ident_Rational
                      ("",static_cast<Ident_Rational*>(op1)->get_value()), Lex(), LEX_TEMP_CONST));
        }
        else if(op2->get_type() == LEX_FLOAT)
        {
            static_cast<Ident_Rational*>(op1)->set_value
                    (static_cast<Ident_Float*>(op2)->get_value());
            pol.push(new Poliz_Const
                     (new Ident_Rational
                      ("",static_cast<Ident_Rational*>(op1)->get_value()), Lex(), LEX_TEMP_CONST));
        }
        else
        {
            if(p1->get_type() == LEX_TEMP_CONST)
                delete p1;
            if(p2->get_type() == LEX_TEMP_CONST)
                delete p2;
            throw "no such operand for " + lex_to_string(op1->get_type()) + " " + lex_to_string(op2->get_type());
        }
        break;
    case LEX_VECTOR:
        if(op2->get_type() == LEX_VECTOR)
        {
            static_cast<Ident_Vector*>(op1)->set_rotation(static_cast<Ident_Vector*>(op2)->get_rotation());
            static_cast<Ident_Vector*>(op1)->set_value
                    (static_cast<Ident_Vector*>(op2)->get_value());
            pol.push(new Poliz_Const
                     (new Ident_Vector
                      ("",static_cast<Ident_Vector*>(op1)->get_value()), Lex(), LEX_TEMP_CONST));
        }
        else
        {
            if(p1->get_type() == LEX_TEMP_CONST)
                delete p1;
            if(p2->get_type() == LEX_TEMP_CONST)
                delete p2;
            throw "no such operand for " + lex_to_string(op1->get_type()) + " " + lex_to_string(op2->get_type());
        }
        break;
    case LEX_MATRIX:
        if(op2->get_type() == LEX_VECTOR)
        {
            static_cast<Ident_Matrix*>(op1)->set_value
                    (Matrix
                     (static_cast<Ident_Vector*>(op2)->get_value(), static_cast<Ident_Vector*>(op2)->get_rotation()));
            pol.push(new Poliz_Const
                     (new Ident_Matrix
                      ("",static_cast<Ident_Matrix*>(op1)->get_value()), Lex(), LEX_TEMP_CONST));
        }
        else if(op2->get_type() == LEX_MATRIX)
        {
            static_cast<Ident_Matrix*>(op1)->set_value
                    (static_cast<Ident_Matrix*>(op2)->get_value());
            pol.push(new Poliz_Const
                     (new Ident_Matrix
                      ("",static_cast<Ident_Matrix*>(op1)->get_value()), Lex(), LEX_TEMP_CONST));
        }
        else
        {
            if(p1->get_type() == LEX_TEMP_CONST)
                delete p1;
            if(p2->get_type() == LEX_TEMP_CONST)
                delete p2;
            throw "no such operand for " + lex_to_string(op1->get_type()) + " " + lex_to_string(op2->get_type());
        }
        break;
    default:
        if(p1->get_type() == LEX_TEMP_CONST)
            delete p1;
        if(p2->get_type() == LEX_TEMP_CONST)
            delete p2;
        throw "no such operand for " + lex_to_string(op1->get_type()) + " " + lex_to_string(op2->get_type());
        break;
    }
    if(p1->get_type() == LEX_TEMP_CONST)
        delete p1;
    if(p2->get_type() == LEX_TEMP_CONST)
        delete p2;
}
void Poliz_Function::power(std::stack<Poliz *> &pol)
{
    Poliz *p2 = pol.top();
    Ident *op2 = pol.top()->get_value();
    pol.pop();
    Poliz *p1 = pol.top();
    Ident *op1 = pol.top()->get_value();
    pol.pop();
    int power = 0;
    switch (op1->get_type()) {
    case LEX_INTEGER:
        if(op2->get_type() == LEX_INTEGER)
        {
            power = static_cast<Ident_Int*>(op2)->get_value();
            if(power < 0)
                pol.push(new Poliz_Const
                         (new Ident_Rational
                          ("", Rational_number(static_cast<Ident_Int*>(op1)->get_value()) ^ power),
                          Lex(), LEX_TEMP_CONST));
            else
                pol.push(new Poliz_Const
                         (new Ident_Int
                          ("", pow(static_cast<Ident_Int*>(op1)->get_value(), power)),
                          Lex(), LEX_TEMP_CONST));
        }
        else if(op2->get_type() == LEX_RATIONAL)
        {
            power = static_cast<Ident_Rational*>(op2)->get_value().get_int();
            if(power < 0)
                pol.push(new Poliz_Const
                         (new Ident_Rational
                          ("", Rational_number(static_cast<Ident_Int*>(op1)->get_value()) ^ power),
                          Lex(), LEX_TEMP_CONST));
            else
                pol.push(new Poliz_Const
                         (new Ident_Int
                          ("", pow(static_cast<Ident_Int*>(op1)->get_value(), power)),
                          Lex(), LEX_TEMP_CONST));
        }
        else
        {
            if(p1->get_type() == LEX_TEMP_CONST)
                delete p1;
            if(p2->get_type() == LEX_TEMP_CONST)
                delete p2;
            throw "no such operand for " + lex_to_string(op1->get_type()) + " " + lex_to_string(op2->get_type());
        }
        break;
    case LEX_FLOAT:
        if(op2->get_type() == LEX_INTEGER)
        {
            power = static_cast<Ident_Int*>(op2)->get_value();
            if(power < 0)
            {
                if(static_cast<Ident_Float*>(op1)->get_value() == 0)
                {
                    if(p1->get_type() == LEX_TEMP_CONST)
                        delete p1;
                    if(p2->get_type() == LEX_TEMP_CONST)
                        delete p2;
                    throw string("Zero devision");
                }
                pol.push(new Poliz_Const
                         (new Ident_Float
                          ("", pow(static_cast<Ident_Float*>(op1)->get_value(), power)),
                          Lex(), LEX_TEMP_CONST));
            }
            else
                pol.push(new Poliz_Const
                         (new Ident_Float
                          ("", pow(static_cast<Ident_Float*>(op1)->get_value(), power)),
                          Lex(), LEX_TEMP_CONST));
        }
        else if(op2->get_type() == LEX_RATIONAL)
        {
            power = static_cast<Ident_Rational*>(op2)->get_value().get_int();
            if(power < 0)
            {
                if(static_cast<Ident_Float*>(op1)->get_value() == 0)
                {
                    if(p1->get_type() == LEX_TEMP_CONST)
                        delete p1;
                    if(p2->get_type() == LEX_TEMP_CONST)
                        delete p2;
                    throw string("Zero devision");
                }
                pol.push(new Poliz_Const
                         (new Ident_Float
                          ("", pow(static_cast<Ident_Float*>(op1)->get_value(), power)),
                          Lex(), LEX_TEMP_CONST));
            }
            else
                pol.push(new Poliz_Const
                         (new Ident_Float
                          ("", pow(static_cast<Ident_Float*>(op1)->get_value(), power)),
                          Lex(), LEX_TEMP_CONST));
        }
        else
        {
            if(p1->get_type() == LEX_TEMP_CONST)
                delete p1;
            if(p2->get_type() == LEX_TEMP_CONST)
                delete p2;
            throw "no such operand for " + lex_to_string(op1->get_type()) + " " + lex_to_string(op2->get_type());
        }
        break;
    case LEX_RATIONAL:
        if(op2->get_type() == LEX_INTEGER)
        {
            power = static_cast<Ident_Int*>(op2)->get_value();
            pol.push(new Poliz_Const
                     (new Ident_Rational
                      ("", static_cast<Ident_Rational*>(op1)->get_value() ^ power),
                      Lex(), LEX_TEMP_CONST));
        }
        else if(op2->get_type() == LEX_RATIONAL)
        {
            power = static_cast<Ident_Rational*>(op2)->get_value().get_int();
            pol.push(new Poliz_Const
                     (new Ident_Rational
                      ("", static_cast<Ident_Rational*>(op1)->get_value() ^ power),
                      Lex(), LEX_TEMP_CONST));
        }
        else
        {
            if(p1->get_type() == LEX_TEMP_CONST)
                delete p1;
            if(p2->get_type() == LEX_TEMP_CONST)
                delete p2;
            throw "no such operand for " + lex_to_string(op1->get_type()) + " " + lex_to_string(op2->get_type());
        }
        break;
    case LEX_MATRIX:
        if(op2->get_type() == LEX_INTEGER)
        {
            if((power = static_cast<Ident_Int*>(op2)->get_value()) < 0)
            {
                if(p1->get_type() == LEX_TEMP_CONST)
                    delete p1;
                if(p2->get_type() == LEX_TEMP_CONST)
                    delete p2;
                throw string("matrix in negative power");
            }
            pol.push(new Poliz_Const
                     (new Ident_Matrix
                      ("", static_cast<Ident_Matrix*>(op1)->get_value() ^ power),
                      Lex(), LEX_TEMP_CONST));
        }
        else if(op2->get_type() == LEX_RATIONAL)
        {
            if((power = static_cast<Ident_Rational*>(op2)->get_value().get_int()) < 0)
            {
                if(p1->get_type() == LEX_TEMP_CONST)
                    delete p1;
                if(p2->get_type() == LEX_TEMP_CONST)
                    delete p2;
                throw string("matrix in negative power");
            }
            pol.push(new Poliz_Const
                     (new Ident_Matrix
                      ("", static_cast<Ident_Matrix*>(op1)->get_value() ^ power),
                      Lex(), LEX_TEMP_CONST));
        }
        else
        {
            if(p1->get_type() == LEX_TEMP_CONST)
                delete p1;
            if(p2->get_type() == LEX_TEMP_CONST)
                delete p2;
            throw "no such operand for " + lex_to_string(op1->get_type()) + " " + lex_to_string(op2->get_type());
        }
        break;
    default:
        if(p1->get_type() == LEX_TEMP_CONST)
            delete p1;
        if(p2->get_type() == LEX_TEMP_CONST)
            delete p2;
        throw "no such operand for " + lex_to_string(op1->get_type()) + " " + lex_to_string(op2->get_type());
        break;
    }
    if(p1->get_type() == LEX_TEMP_CONST)
        delete p1;
    if(p2->get_type() == LEX_TEMP_CONST)
        delete p2;

}
void Poliz_Function::row(std::stack<Poliz *> &pol)
{
    Poliz *p2 = pol.top();
    Ident *op2 = pol.top()->get_value();
    pol.pop();
    Poliz *p1 = pol.top();
    Ident *op1 = pol.top()->get_value();
    pol.pop();
    switch (op2->get_type()) {
    case LEX_MATRIX:
        if(op1->get_type() == LEX_INTEGER)
            pol.push(new Poliz_Const
                     (new Ident_Vector
                      ("", static_cast<Ident_Matrix*>(op2)->get_value().get_row
                       (static_cast<Ident_Int*>(op1)->get_value())),
                      Lex(), LEX_TEMP_CONST));
        else if(op1->get_type() == LEX_RATIONAL)
            pol.push(new Poliz_Const
                     (new Ident_Vector
                      ("", static_cast<Ident_Matrix*>(op2)->get_value().get_row
                       (static_cast<Ident_Rational*>(op1)->get_value().get_int())),
                      Lex(), LEX_TEMP_CONST));
        else
        {
            if(p1->get_type() == LEX_TEMP_CONST)
                delete p1;
            if(p2->get_type() == LEX_TEMP_CONST)
                delete p2;
            throw "no such operand for " + lex_to_string(op1->get_type()) + " " + lex_to_string(op2->get_type());
        }
        break;
    default:
        if(p1->get_type() == LEX_TEMP_CONST)
            delete p1;
        if(p2->get_type() == LEX_TEMP_CONST)
            delete p2;
        throw "no such operand for " + lex_to_string(op1->get_type()) + " " + lex_to_string(op2->get_type());
        break;
    }

    if(p1->get_type() == LEX_TEMP_CONST)
        delete p1;
    if(p2->get_type() == LEX_TEMP_CONST)
        delete p2;
}
void Poliz_Function::column(std::stack<Poliz *> &pol)
{
    Poliz *p2 = pol.top();
    Ident *op2 = pol.top()->get_value();
    pol.pop();
    Poliz *p1 = pol.top();
    Ident *op1 = pol.top()->get_value();
    pol.pop();
    switch (op2->get_type()) {
    case LEX_MATRIX:
        if(op1->get_type() == LEX_INTEGER)
            pol.push(new Poliz_Const
                     (new Ident_Vector
                      ("", static_cast<Ident_Matrix*>(op2)->get_value().get_column
                       (static_cast<Ident_Int*>(op1)->get_value())),
                      Lex(), LEX_TEMP_CONST));
        else if(op1->get_type() == LEX_RATIONAL)
            pol.push(new Poliz_Const
                     (new Ident_Vector
                      ("", static_cast<Ident_Matrix*>(op2)->get_value().get_column
                       (static_cast<Ident_Rational*>(op1)->get_value().get_int())),
                      Lex(), LEX_TEMP_CONST));
        else
        {
            if(p1->get_type() == LEX_TEMP_CONST)
                delete p1;
            if(p2->get_type() == LEX_TEMP_CONST)
                delete p2;
            throw "no such operand for " + lex_to_string(op1->get_type()) + " " + lex_to_string(op2->get_type());
        }
        break;
    default:
        if(p1->get_type() == LEX_TEMP_CONST)
            delete p1;
        if(p2->get_type() == LEX_TEMP_CONST)
            delete p2;
        throw "no such operand for " + lex_to_string(op1->get_type()) + " " + lex_to_string(op2->get_type());
        break;
    }
    if(p1->get_type() == LEX_TEMP_CONST)
        delete p1;
    if(p2->get_type() == LEX_TEMP_CONST)
        delete p2;
}
void Poliz_Function::make_canonical(std::stack<Poliz *> &pol)
{
    Poliz *p = pol.top();
    Ident *op = pol.top()->get_value();
    pol.pop();
    Rational_number r;
    Vector v;
    Matrix m;
    switch (op->get_type()) {
    case LEX_RATIONAL:
        r = static_cast<Ident_Rational*>(op)->get_value();
        r.make_canonical();
        pol.push(new Poliz_Const
                 (new Ident_Rational
                  ("", r), Lex(), LEX_TEMP_CONST));
        break;
    case LEX_VECTOR:
        v = static_cast<Ident_Vector*>(op)->get_value();
        v.make_canonical();
        pol.push(new Poliz_Const
                 (new Ident_Vector
                  ("", v), Lex(), LEX_TEMP_CONST));
        break;
    case LEX_MATRIX:
        m = static_cast<Ident_Vector*>(op)->get_value();
        m.make_canonical();
        pol.push(new Poliz_Const
                 (new Ident_Matrix
                  ("", m), Lex(), LEX_TEMP_CONST));
        break;
    default:
        if(p->get_type() == LEX_TEMP_CONST)
            delete p;
        throw "no such operand for " + lex_to_string(op->get_type());
        break;
    }
    if(p->get_type() == LEX_TEMP_CONST)
        delete p;
}
void Poliz_Function::rotate(std::stack<Poliz *> &pol)
{
    Poliz *p = pol.top();
    Ident *op = pol.top()->get_value();
    pol.pop();
    switch (op->get_type()) {
    case LEX_VECTOR:
        pol.push(new Poliz_Const
                 (new Ident_Vector
                  ("", static_cast<Ident_Vector*>(op)->get_value(),
                   !static_cast<Ident_Vector*>(op)->get_rotation()),
                  Lex(), LEX_TEMP_CONST));
        break;
    case LEX_MATRIX:
        pol.push(new Poliz_Const
                 (new Ident_Matrix
                  ("", static_cast<Ident_Matrix*>(op)->get_value().operator ~()),
                  Lex(), LEX_TEMP_CONST));
        break;
    default:
        if(p->get_type() == LEX_TEMP_CONST)
            delete p;
        throw "no such operand for " + lex_to_string(op->get_type());
        break;
    }
    if(p->get_type() == LEX_TEMP_CONST)
        delete p;
}
void Poliz_Function::minus(std::stack<Poliz *> &pol)
{
    Poliz *p = pol.top();
    Ident *op = pol.top()->get_value();
    pol.pop();
    switch (op->get_type()) {
    case LEX_INTEGER:
        pol.push(new Poliz_Const
                 (new Ident_Int
                  ("", -static_cast<Ident_Int*>(op)->get_value()),
                  Lex(), LEX_TEMP_CONST));
        break;
    case LEX_FLOAT:
        pol.push(new Poliz_Const
                 (new Ident_Float
                  ("", -static_cast<Ident_Float*>(op)->get_value()),
                  Lex(), LEX_TEMP_CONST));
        break;
    case LEX_RATIONAL:
        pol.push(new Poliz_Const
                 (new Ident_Rational
                  ("", -static_cast<Ident_Rational*>(op)->get_value()),
                  Lex(), LEX_TEMP_CONST));
        break;
    case LEX_VECTOR:
        pol.push(new Poliz_Const
                 (new Ident_Vector
                  ("", -static_cast<Ident_Vector*>(op)->get_value()),
                  Lex(), LEX_TEMP_CONST));
        break;
    case LEX_MATRIX:
        pol.push(new Poliz_Const
                 (new Ident_Matrix
                  ("", -static_cast<Ident_Matrix*>(op)->get_value()),
                  Lex(), LEX_TEMP_CONST));
        break;
    default:
        if(p->get_type() == LEX_TEMP_CONST)
            delete p;
        throw "no such operand for " + lex_to_string(op->get_type());
        break;
    }
    if(p->get_type() == LEX_TEMP_CONST)
        delete p;
}
void Poliz_Function::write(std::stack<Poliz *> &pol)
{
    Poliz *p2 = pol.top();
    Ident *op2 = pol.top()->get_value();
    pol.pop();
    Poliz *p1 = pol.top();
    Ident *op1 = pol.top()->get_value();
    pol.pop();
    ofstream myfile;
    if(op2->get_type() != LEX_STRING)
    {
        if(p1->get_type() == LEX_TEMP_CONST)
            delete p1;
        if(p2->get_type() == LEX_TEMP_CONST)
            delete p2;
        throw "no such operand for " + lex_to_string(op1->get_type()) + " " + lex_to_string(op2->get_type());
    }
    myfile.open (static_cast<Ident_String*>(op2)->get_value());
    if(!myfile.is_open())
    {
        if(p1->get_type() == LEX_TEMP_CONST)
            delete p1;
        if(p2->get_type() == LEX_TEMP_CONST)
            delete p2;
        throw string("unable to open ") + static_cast<Ident_String*>(op2)->get_value();
    }
    switch (op1->get_type()) {
    case LEX_INTEGER:
        myfile << static_cast<Ident_Int*>(op1)->get_value();
        myfile.close();
        break;
    case LEX_FLOAT:
        myfile << static_cast<Ident_Float*>(op1)->get_value();
        myfile.close();
        break;
    case LEX_RATIONAL:
        myfile << static_cast<Ident_Rational*>(op1)->get_value();
        myfile.close();
        break;
    case LEX_VECTOR:
        myfile.close();
        static_cast<Ident_Vector*>(op1)->get_value().write
                (static_cast<Ident_String*>(op2)->get_value().c_str(),
                 static_cast<Ident_Vector*>(op1)->get_rotation());
        break;
    case LEX_MATRIX:
        myfile.close();
        static_cast<Ident_Matrix*>(op1)->get_value().write
                (static_cast<Ident_String*>(op2)->get_value().c_str());
        break;
    default:
        if(p1->get_type() == LEX_TEMP_CONST)
            delete p1;
        if(p2->get_type() == LEX_TEMP_CONST)
            delete p2;
        throw "no such operand for " + lex_to_string(op1->get_type()) + " " + lex_to_string(op2->get_type());
        break;
    }
    pol.push(p1);
    if(p2->get_type() == LEX_TEMP_CONST)
        delete p2;
}
void Poliz_Function::read(std::stack<Poliz *> &pol)
{
    Poliz *p2 = pol.top();
    Ident *op2 = pol.top()->get_value();
    pol.pop();
    Poliz *p1 = pol.top();
    Ident *op1 = pol.top()->get_value();
    pol.pop();
    if(p1->get_type() != LEX_VAR)
    {
        if(p1->get_type() == LEX_TEMP_CONST)
            delete p1;
        if(p2->get_type() == LEX_TEMP_CONST)
            delete p2;
        throw string("left operand must be non const");
    }
    if(op2->get_type() != LEX_STRING)
    {
        if(p1->get_type() == LEX_TEMP_CONST)
            delete p1;
        if(p2->get_type() == LEX_TEMP_CONST)
            delete p2;
        throw "no such operand for " + lex_to_string(op1->get_type()) + " " + lex_to_string(op2->get_type());
    }
    switch (op1->get_type()) {
    case LEX_VECTOR:
        static_cast<Ident_Vector*>(op1)->set_value
                (Vector(static_cast<Ident_String*>(op2)->get_value().c_str()));
        pol.push(new Poliz_Const
                 (new Ident_Vector
                  ("",static_cast<Ident_Vector*>(op1)->get_value()), Lex(), LEX_TEMP_CONST));
        break;
    case LEX_MATRIX:
        static_cast<Ident_Matrix*>(op1)->set_value
                (Vector(static_cast<Ident_String*>(op2)->get_value().c_str()));
        pol.push(new Poliz_Const
                 (new Ident_Matrix
                  ("",static_cast<Ident_Matrix*>(op1)->get_value()), Lex(), LEX_TEMP_CONST));
        break;
    default:
        if(p1->get_type() == LEX_TEMP_CONST)
            delete p1;
        if(p2->get_type() == LEX_TEMP_CONST)
            delete p2;
        throw "no such operand for " + lex_to_string(op1->get_type()) + " " + lex_to_string(op2->get_type());
        break;
    }
    if(p1->get_type() == LEX_TEMP_CONST)
        delete p1;
    if(p2->get_type() == LEX_TEMP_CONST)
        delete p2;
}
void Poliz_Function::comma(std::stack<Poliz *> &pol)
{
    Poliz *p2 = pol.top();
    Ident *op2 = pol.top()->get_value();
    pol.pop();
    Poliz *p1 = pol.top();
    Ident *op1 = pol.top()->get_value();
    pol.pop();
    Ident_Comma* com = nullptr;
    switch (op1->get_type()) {
    case LEX_INTEGER:
        com = new Ident_Comma();
        com->add(static_cast<Ident_Int*>(op1)->get_value());
        if(op2->get_type() == LEX_INTEGER)
            com->add(static_cast<Ident_Int*>(op2)->get_value());
        else if(op2->get_type() == LEX_RATIONAL)
            com->add(static_cast<Ident_Rational*>(op2)->get_value().get_int());
        else
        {
            delete com;
            if(p1->get_type() == LEX_TEMP_CONST)
                delete p1;
            if(p2->get_type() == LEX_TEMP_CONST)
                delete p2;
            throw "no such operand for " + lex_to_string(op1->get_type()) + " " + lex_to_string(op2->get_type());
        }
        pol.push(new Poliz_Const
                 (com, Lex(), LEX_TEMP_CONST));
        break;
    case LEX_RATIONAL:
        com = new Ident_Comma();
        com->add(static_cast<Ident_Rational*>(op1)->get_value().get_int());
        if(op2->get_type() == LEX_INTEGER)
            com->add(static_cast<Ident_Int*>(op2)->get_value());
        else if(op2->get_type() == LEX_RATIONAL)
            com->add(static_cast<Ident_Rational*>(op2)->get_value().get_int());
        else
        {
            delete com;
            if(p1->get_type() == LEX_TEMP_CONST)
                delete p1;
            if(p2->get_type() == LEX_TEMP_CONST)
                delete p2;
            throw "no such operand for " + lex_to_string(op1->get_type()) + " " + lex_to_string(op2->get_type());
        }
        pol.push(new Poliz_Const
                 (com, Lex(), LEX_TEMP_CONST));
        break;
    case LEX_COMMA:
        com = new Ident_Comma(static_cast<Ident_Comma*>(op1)->get_value());
        if(op2->get_type() == LEX_INTEGER)
            com->add(static_cast<Ident_Int*>(op2)->get_value());
        else if(op2->get_type() == LEX_RATIONAL)
            com->add(static_cast<Ident_Rational*>(op2)->get_value().get_int());
        else
        {
            delete com;
            if(p1->get_type() == LEX_TEMP_CONST)
                delete p1;
            if(p2->get_type() == LEX_TEMP_CONST)
                delete p2;
            throw "no such operand for " + lex_to_string(op1->get_type()) + " " + lex_to_string(op2->get_type());
        }
        pol.push(new Poliz_Const
                 (com, Lex(), LEX_TEMP_CONST));
        break;
    default:
        if(p1->get_type() == LEX_TEMP_CONST)
            delete p1;
        if(p2->get_type() == LEX_TEMP_CONST)
            delete p2;
        throw "no such operand for " + lex_to_string(op1->get_type()) + " " + lex_to_string(op2->get_type());
        break;
    }
    if(p1->get_type() == LEX_TEMP_CONST)
        delete p1;
    if(p2->get_type() == LEX_TEMP_CONST)
        delete p2;
}
void Poliz_Function::get_elem(std::stack<Poliz *> &pol)
{
    Poliz *p2 = pol.top();
    Ident *op2 = pol.top()->get_value();
    pol.pop();
    Poliz *p1 = pol.top();
    Ident *op1 = pol.top()->get_value();
    pol.pop();
    switch (op1->get_type()) {
    case LEX_VECTOR:
        if(op2->get_type() == LEX_RATIONAL)
            pol.push(new Poliz_Const
                     (new Ident_Rational
                      ("", static_cast<Ident_Vector*>(op1)->get_value()
                       [static_cast<Ident_Rational*>(op2)->get_value().get_int()]),
                     Lex(), LEX_TEMP_CONST));
        else if(op2->get_type() == LEX_INTEGER)
            pol.push(new Poliz_Const
                     (new Ident_Rational
                      ("", static_cast<Ident_Vector*>(op1)->get_value()
                       [static_cast<Ident_Int*>(op2)->get_value()]),
                     Lex(), LEX_TEMP_CONST));
        else if(op2->get_type() == LEX_COMMA)
        {
            if(p1->get_type() == LEX_TEMP_CONST)
                delete p1;
            if(p2->get_type() == LEX_TEMP_CONST)
                delete p2;
            throw string("too many indexes");
        }
        else
        {
            if(p1->get_type() == LEX_TEMP_CONST)
                delete p1;
            if(p2->get_type() == LEX_TEMP_CONST)
                delete p2;
            throw "no such operand for " + lex_to_string(op1->get_type()) + " " + lex_to_string(op2->get_type());
        }
        break;
    case LEX_MATRIX:
        switch (op2->get_type()) {
        case LEX_COMMA:
            if(static_cast<Ident_Comma*>(op2)->get_length() != 2)
            {
                if(p1->get_type() == LEX_TEMP_CONST)
                    delete p1;
                if(p2->get_type() == LEX_TEMP_CONST)
                    delete p2;
                throw string("too many indexes");
            }
            if(static_cast<Ident_Comma*>(op2)->get_elem(0) < 0 ||
                    static_cast<Ident_Comma*>(op2)->get_elem(0) < 0)
            {
                if(p1->get_type() == LEX_TEMP_CONST)
                    delete p1;
                if(p2->get_type() == LEX_TEMP_CONST)
                    delete p2;
                throw string("negative index");
            }
            pol.push(new Poliz_Const
                     (new Ident_Rational
                      ("", static_cast<Ident_Matrix*>(op1)->get_value()
                       [static_cast<Ident_Comma*>(op2)->get_elem(0)]
                       [static_cast<Ident_Comma*>(op2)->get_elem(1)]),
                     Lex(), LEX_TEMP_CONST));
            break;
        default:
            if(p1->get_type() == LEX_TEMP_CONST)
                delete p1;
            if(p2->get_type() == LEX_TEMP_CONST)
                delete p2;
            throw string("not enough indexes");
            break;
        }
        break;
    default:
        if(p1->get_type() == LEX_TEMP_CONST)
            delete p1;
        if(p2->get_type() == LEX_TEMP_CONST)
            delete p2;
        throw "no such operand for " + lex_to_string(op1->get_type()) + " " + lex_to_string(op2->get_type());
        break;
    }
    if(p1->get_type() == LEX_TEMP_CONST)
        delete p1;
    if(p2->get_type() == LEX_TEMP_CONST)
        delete p2;
}
void Poliz_Function::print(std::stack<Poliz*>& pol)
{
    Ident *op = pol.top()->get_value();
    switch (op->get_type())
    {
    case LEX_INTEGER:
        cout << static_cast<Ident_Int*>(op)->get_value() << endl;
        break;
    case LEX_FLOAT:
        cout << static_cast<Ident_Float*>(op)->get_value() << endl;
        break;
    case LEX_RATIONAL:
        cout << static_cast<Ident_Rational*>(op)->get_value() << endl;
        break;
    case LEX_VECTOR:
        cout << static_cast<Ident_Vector*>(op)->get_value() << endl;
        break;
    case LEX_MATRIX:
        cout << static_cast<Ident_Matrix*>(op)->get_value() << endl;
        break;
    case LEX_STRING:
        cout << static_cast<Ident_String*>(op)->get_value() << endl;
        break;
    default:
        throw "no such operand for " + lex_to_string(op->get_type());
    break;
    }
}
void Poliz_Function::semicolon(std::stack<Poliz *> &pol)
{
    Poliz *p;
    while (!pol.empty())
    {
        if((p = pol.top())->get_type() == LEX_TEMP_CONST)
            delete p;
        pol.pop();
    }
}
void Poliz_Function::execute(std::stack<Poliz *> &pol)
{
    try
    {
        switch (oper) {
        case LEX_PLUS:
            add(pol);
            break;
        case LEX_MINUS:
            sub(pol);
            break;
        case LEX_STAR:
            mul(pol);
            break;
        case LEX_SLASH:
            div(pol);
            break;
        case LEX_EQUAL:
            equal(pol);
            break;
        case LEX_POWER:
            power(pol);
            break;
        case LEX_ROTATE:
            rotate(pol);
            break;
        case LEX_WRITE:
            write(pol);
            break;
        case LEX_READ:
            read(pol);
            break;
        case LEX_ROW:
            row(pol);
            break;
        case LEX_COLUMN:
            column(pol);
            break;
        case LEX_CANONICAL:
            make_canonical(pol);
            break;
        case LEX_UNARY_MINUS:
            minus(pol);
            break;
        case LEX_GET_ELEM:
            get_elem(pol);
            break;
        case LEX_COMMA:
            comma(pol);
            break;
        case LEX_SEMICOLON:
            semicolon(pol);
            break;
        case LEX_PRINT:
            print(pol);
            break;
        default:
            throw string("smth wrong in execute");
            break;
        }
    }
    catch(string &ex)
    {
        semicolon(pol);
        throw  ":" + to_string(this->get_lexem().get_line()) + ":" +
                to_string(this->get_lexem().get_column()) + ": error: in operator '" +
                lex_to_string(oper) + "' " + ex;
    }
}
void Poliz_Info::execute(std::stack<Poliz *> &pol)
{
    if(pol.size() || !pol.size())
        cout << str;
}
