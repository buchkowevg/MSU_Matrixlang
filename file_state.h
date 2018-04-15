#ifndef FILE_CONDITION_H
#define FILE_CONDITION_H
#include <vector>
#include <fstream>
#include "ident.h"
class File_state
{
    std::ifstream input_file;
    unsigned int line, column;
    std::vector<Ident*> *variables;

public:
    File_state(const std::ifstream &f, const int &i = 0, const int &j = 0)
    {
        //input_file = f;
        line = i;
        column = j;
    }
    //std::ifstream get_file() const { return input_file; }
    //void set_file( const std::ifstream &f) { input_file = f; }
    const unsigned int& get_line() const { return line; }
    const unsigned int& get_column() const { return column; }
    void set_column(unsigned int &i) { column = i; }
    void set_line(unsigned int &j) { line = j; }
};
#endif // FILE_CONDITION_H
