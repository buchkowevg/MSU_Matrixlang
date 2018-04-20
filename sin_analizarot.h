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

    void gl();
    void process();
    void declare();
    void declaration();
public:
    Sin_analizarot(const std::vector<Lex>&, const std::string&);
    void parse();
};
#endif // SIN_ANALIZAROT_H
