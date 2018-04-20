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
 /*   map<string , Ident*> m;
    Ident_Int *idI =new Ident_Int("a", 12);
    Ident_Rational *idR= new Ident_Rational("b", Rational_number("1/2"));
    m.insert(pair<string, Ident*> (idI->get_name(), idI));
    m.insert(pair<string, Ident*> (idR->get_name(), idR));
    Ident *i0 = m.find("a")->second;
    Ident *i1 = m.find("b")->second;
    Ident_Rational *rat = static_cast<Ident_Rational*>(i1);
    Ident_Int *b = static_cast<Ident_Int*>(i0);
    cout << *rat << endl << *b << endl;
    b->set_value(1377);
    b = static_cast<Ident_Int*>(m.find("a")->second);
    cout << *b << endl << *rat << endl;
 */
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

    return 0;
}
