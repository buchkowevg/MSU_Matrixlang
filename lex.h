#ifndef LEX_H
#define LEX_H
#include <ostream>
enum type_of_lex
{
    LEX_NULL,
    LEX_DECLARE,
    LEX_PROCESS,
    LEX_COMMA,
    LEX_OPENED_BRACKET,
    LEX_CLOSED_BRACKET,
    LEX_QUOTE,
    LEX_COLON,
    LEX_SEMICOLON,
    LEX_FLOAT,
    LEX_RATIONAL,
    LEX_MATRIX,
    LEX_VECTOR,
    LEX_INTEGER
};

class Lex
{
public:
    Lex(type_of_lex t = LEX_NULL, int v = 0)
    {
        t_lex = t;
        v_lex = v;
    }
    type_of_lex get_type() const
    {
        return t_lex;
    }
    int get_value() const
    {
        return v_lex;
    }
    friend std::ostream& operator <<(std::ostream& s, Lex l)
    {
        s << '(' << l.t_lex << ',' << l.v_lex << ");";
        return s;
    }
private:
    type_of_lex t_lex;
    int v_lex;
};

#endif // LEX_H
