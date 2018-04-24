#ifndef DECLARE_PARSER_H
#define DECLARE_PARSER_H
#include <fstream>
#include <vector>
#include <string>
#include "ident.h"

class Lex_analizator
{
    unsigned int line, column, prevline, prevcolumn;
    std::ifstream file;
    std::vector<Lex> lex_vector;
    std::string filename;
    static char TD[];
    static std::string TW[];

    type_of_lex lookTD(const char&) const;
    type_of_lex lookTW(const std::string&) const;
    char skipNS(char);
    void linecolumn(char);
    char sharp_comm();
    char big_comm();
    std::string quotes();
    bool isdigit(char) const;
    char gc();
    void ungc(char);
public:    
    Lex_analizator();

    const std::vector<Lex>& get_vector() const;
    void open(const std::string&);
    void write(const std::string&) const;
    void print() const;
    void parse();
};

#endif // DECLARE_PARSER_H
