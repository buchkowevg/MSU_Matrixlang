#ifndef SIN_ANALIZAROT_H
#define SIN_ANALIZAROT_H

#include <unordered_map>
#include <vector>
#include <stack>

#include "lex.h"
#include "ident.h"
#include "poliz.h"
class Sin_analizarot
{
    std::vector<Lex> lex_vector;
    std::string filename;
    Lex curr_lex;
    type_of_lex curr_type;
    std::unordered_map<std::string , Ident*> var_map;
    std::vector<Lex> dec_vector;
    std::vector<Poliz*> poliz_vector;
    std::stack<Lex> poliz_stack;

    void exeption(const std::string&) const;
    std::string get_dec_str() const;
    unsigned int check_your_privilege(type_of_lex) const;
    bool isfunction(type_of_lex) const;
    bool isvar_const(type_of_lex) const;
    bool isoper(type_of_lex) const;
    void push_dec(std::string &, unsigned int, unsigned int );
    void gl();
    void add_to_poliz();

    void declare();
    void declaration();
    void constructor();
    void constructor1();

    void process();
    void process1();
    void process2();
    void process_oper();
    void bracket();
    void bracket_oper();
    void square_bracket();
    void square_oper();
    void info();
    void function();
    void unary();

public:
    Sin_analizarot(const std::vector<Lex>&, const std::string&);
    ~Sin_analizarot();
    void write_var_map(const std::string&) const;
    void write_poliz(const std::string&) const;
    void parse();
    void execute();
};
#endif // SIN_ANALIZAROT_H
