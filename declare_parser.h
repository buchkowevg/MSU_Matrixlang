#ifndef DECLARE_PARSER_H
#define DECLARE_PARSER_H
#include <fstream>
#include <vector>
#include <string>
#include "ident.h"

class Declare_parser
{
    unsigned int line, column, prevline, prevcolumn;
    std::ifstream file;
    std::vector<Lex> lex_vector;
    enum state{START, COMSHARP, COMSLASH, DEC, VAR, NAME, CONSTRUCTOR, STRING};
    state CS;
    static char TD[];
    static std::string TW[];

    type_of_lex lookTD(const char&) const;
    type_of_lex lookTW(const std::string&) const;
    char skipNS(char c);
    void linecolumn(char c);
    char sharp_comm();
    char big_comm();
    std::string quotes();
    bool isdigit(const std::string&) const;
public:    

    void write(const std::string&) const;
    void print() const;
    Declare_parser(const std::string &);
    void parse();
};

#endif // DECLARE_PARSER_H
