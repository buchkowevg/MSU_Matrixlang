#include <iostream>
#include "ident.h"
#include <vector>
#include <string>
#include "../sparse/rational_number.h"
#include <fstream>
#include <map>
#include "lex_analizator.h"
#include "sin_analizarot.h"
using namespace std;
int main(int argc, char** argv)
{
    //TODO matrix bad file format
    if(argc < 2)
    {
        cerr << "Not enough arguments" << endl;
        return 1;
    }
    Lex_analizator d;
    try
    {
        d.open(argv[1]);
        d.parse();
        d.write("lex_log.txt");
    }
    catch(const string &ex)
    {
        cerr << ex << endl;
        return 1;
    }
    Sin_analizarot s(d.get_vector(), argv[1]);
    try
    {
        s.parse();
        s.write_var_map("var_map.txt");
    }
    catch(const string &ex)
    {
        cerr << ex << endl;
        return 1;
    }

    return 0;
}
