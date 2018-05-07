#include "poliz.h"

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
Poliz_Const::Poliz_Const(Ident *id, Lex l) : Poliz(LEX_CONST, l)
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
