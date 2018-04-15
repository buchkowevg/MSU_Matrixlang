#include <iostream>
#include "ident.h"
#include <vector>
#include "../sparse/rational_number.h"
using namespace std;

int main()
{
    vector<Ident*> v;
    v.push_back(new Ident_Int("a", 12));
    v.push_back(new Ident_Rational("b", Rational_number("1/2")));
    Ident *i0 = v[0];
    Ident *i1 = v[1];
    Ident_Rational *rat = dynamic_cast<Ident_Rational*>(i1);
    Ident_Int *b = dynamic_cast<Ident_Int*>(i0);
    cout << *rat << endl << *b << endl;
    b->set_value(1377);
    b = dynamic_cast<Ident_Int*>(v[0]);
    cout << *rat << endl << *b << endl;
    return 0;
}
