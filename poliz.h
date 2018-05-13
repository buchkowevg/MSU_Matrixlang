#ifndef POLIZ_H
#define POLIZ_H
#include "ident.h"
#include <stack>
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

    virtual void execute(std::stack<Poliz*> &) = 0;
    virtual Ident* get_value() const = 0;
    virtual void set_value(Ident*) = 0;
    virtual ~Poliz() = 0;
};
class Poliz_Const : public Poliz
{
    Ident *value;
public:
    Poliz_Const();
    Poliz_Const(Ident*, Lex = Lex(), type_of_lex t = LEX_CONST);
    ~Poliz_Const();
    virtual Ident* get_value() const;
    virtual void set_value(Ident*);
    void execute(std::stack<Poliz*> &) { }
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
    void execute(std::stack<Poliz*> &) { }
};
class Poliz_Function : public Poliz
{
    type_of_lex oper;
public:
    Poliz_Function();
    Poliz_Function(type_of_lex, Lex);
    ~Poliz_Function() {}
    type_of_lex get_oper() const;
    void add(std::stack<Poliz*> &);
    void sub(std::stack<Poliz*> &);
    void mul(std::stack<Poliz*> &);
    void div(std::stack<Poliz*> &);
    void equal(std::stack<Poliz*> &);
    void power(std::stack<Poliz*> &);
    void rotate(std::stack<Poliz*> &);
    void write(std::stack<Poliz*> &);
    void read(std::stack<Poliz*> &);
    void print(std::stack<Poliz*> &);
    void row(std::stack<Poliz*> &);
    void column(std::stack<Poliz*> &);
    void make_canonical(std::stack<Poliz*> &);
    void minus(std::stack<Poliz*> &);
    void get_elem(std::stack<Poliz*> &);
    void comma(std::stack<Poliz*> &);
    void semicolon(std::stack<Poliz*> &);
    void execute(std::stack<Poliz*> &);
    virtual Ident* get_value() const { return nullptr; }
    virtual void set_value(Ident*) {}
};
class Poliz_Info : public Poliz
{
    std::string str;
public:
    Poliz_Info();
    Poliz_Info(const std::string&);
    const std::string& get_str() const;
    void set_string(std::string&);
    ~Poliz_Info() {}
    void execute(std::stack<Poliz*> &);
    virtual Ident* get_value() const { return nullptr; }
    virtual void set_value(Ident*) {}
};
#endif // POLIZ_H
