#ifndef POLIZ_H
#define POLIZ_H
#include "ident.h"
#include <forward_list>
#include <iostream>

class Poliz
{
    type_of_lex type;
    Lex lexem;
public:
    Poliz();
    Poliz(type_of_lex);
    Poliz(type_of_lex, Lex);
    type_of_lex get_type() const;
    void set_type(type_of_lex);
    Lex get_lexem() const;
    void set_lexem(Lex&);

    virtual Ident* execute() = 0;
    virtual Ident* get_value() const = 0;
    virtual void set_value(Ident*) = 0;
    virtual ~Poliz() = 0;
};
class Poliz_Const : public Poliz
{
    Ident *value;
public:
    Poliz_Const();
    Poliz_Const(Ident*, Lex);
    ~Poliz_Const();
    virtual Ident* get_value() const;
    virtual void set_value(Ident*);
    Ident* execute() { return nullptr; }
};
class Poliz_Var : public Poliz
{
    Ident *value;
public:
    Poliz_Var();
    Poliz_Var(Ident*, Lex);
    ~Poliz_Var() {}
    Ident* get_value() const;
    void set_value(Ident*);
    Ident* execute() { return nullptr; }
};
class Poliz_Function : public Poliz
{
    type_of_lex oper;
public:
    Poliz_Function();
    Poliz_Function(type_of_lex, Lex);
    ~Poliz_Function() {}
    type_of_lex get_oper() const;
    Ident* add();
    Ident* sub();
    Ident* mul();
    Ident* div();
    Ident* equal();
    Ident* power();
    Ident* write();
    Ident* read();
    Ident* print();
    Ident* row();
    Ident* column();
    Ident* make_canonical();
    Ident* minus();
    Ident* get_elem();
    void comma();
    Ident* execute() { return nullptr; } //TODO
    virtual Ident* get_value() const { return nullptr; }
    virtual void set_value(Ident*) {}
};
#endif // POLIZ_H
