#ifndef SIN_ANALIZAROT_H
#define SIN_ANALIZAROT_H

#include <map>
#include <vector>
#include "lex.h"
#include "ident.h"
class Sin_analizarot
{
    std::vector<Lex> lex_vector;
    std::string filename;
    Lex curr_lex;
    type_of_lex curr_type;
    std::map<std::string , Ident*> var_map;
    std::vector<Lex> dec_vector;

    void exeption(const std::string&) const;
    void push_dec(std::string &, unsigned int, unsigned int );
    std::string get_dec_str() const;

    void constructor();
    void constructor1();
    void gl();
    void process();
    void declare();
    void declaration();
public:
    Sin_analizarot(const std::vector<Lex>&, const std::string&);
    ~Sin_analizarot();
    void write_var_map(const std::string&) const;
    void parse();
};
#endif // SIN_ANALIZAROT_H
