#ifndef DECLARE_PARSER_H
#define DECLARE_PARSER_H
#include <fstream>
#include <vector>
#include <string>
#include "ident.h"
#include "file_state.h"

class Declare_parser
{
    unsigned int line, column;
    std::ifstream file;
    std::vector<Ident*> variables;
    enum state{START, COMSHARP, COMSLASH, DEC, VAR, NAME, CONSTRUCTOR, STRING};
    state CS;
    static std::string TD[];
    static std::string TW[];

    unsigned int look(const std::string&, const std::string*) const;
    char skipNS(char c);
    void linecolumn(char c);
    char sharp_comm();
    char big_comm();
public:    
    Declare_parser(const std::string &);
    File_state& parse();
};

#endif // DECLARE_PARSER_H
