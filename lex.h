#ifndef LEX_H
#define LEX_H
#include <ostream>
#include <string>
enum type_of_lex
{
    LEX_NULL,
    LEX_DECLARE,
    LEX_INTEGER,
    LEX_FLOAT,
    LEX_RATIONAL,
    LEX_VECTOR,
    LEX_MATRIX,
    LEX_PROCESS,
    LEX_INFO,
    LEX_ROTATE,
    LEX_PRINT,
    LEX_WRITE,
    LEX_ROW,
    LEX_COLUMN,
    LEX_CANONICAL,
    LEX_READ,

    LEX_FIN,
    LEX_COMMA,
    LEX_OPEN_ROUND_BRACKET,
    LEX_CLOSE_ROUND_BRACKET,
    LEX_QUOT,
    LEX_SLASH,
    LEX_DOT,
    LEX_SHARP_COMM,
    LEX_STAR,
    LEX_COLON,
    LEX_SEMICOLON,
    LEX_OPEN_SQUARE_BRACKET,
    LEX_CLOSE_SQUARE_BRACKET,
    LEX_PLUS,
    LEX_MINUS,
    LEX_EQUAL,
    LEX_POWER,
    LEX_SPACE,

    LEX_OPEN_COMM,
    LEX_CLOSE_COMM,
    LEX_VAR,
    LEX_STRING,
    LEX_DIGIT,
    LEX_DOUBLE,
    LEX_FUNCTION,
    LEX_CONST,
    LEX_UNARY_MINUS,
    LEX_UNARY_PLUS,
    LEX_GET_ELEM,
    LEX_TEMP_CONST
};

class Lex
{
    type_of_lex type;
    std::string value;
    unsigned int line;
    unsigned int column;
public:
    Lex()
    {
        type = LEX_NULL;
        value = "";
        line = 0;
        column = 0;
    }
    Lex(unsigned int l, unsigned int c, type_of_lex t = LEX_NULL, const std::string &str = "")
    {
        type = t;
        line = l;
        column = c;
        value = str;
    }
    Lex(const Lex &l)
    {
        type = l.type;
        line  = l.line;
        column = l.column;
        value = l.value;
    }
    type_of_lex get_type() const
    {
        return type;
    }
    unsigned int get_line() const
    {
        return line;
    }
    unsigned int get_column() const
    {
        return column;
    }
    const std::string& get_value() const
    {
        return value;
    }
    void set_type(type_of_lex t)
    {
        type = t;
    }
    void set_linecol(unsigned int l, unsigned int c)
    {
        line = l;
        column = c;
    }
    void set_value(const std::string &str)
    {
        value = str;
    }
    void set(type_of_lex t, const std::string &str = "")
    {
        type = t;
        value = str;
    }
    std::string get_str_value(type_of_lex t) const
    {
        switch (t) {
        case LEX_NULL:
            return "NULL";
        case LEX_DECLARE:
            return "declare";
        case LEX_INTEGER:
            return "integer";
        case LEX_FLOAT:
            return "float";
        case LEX_RATIONAL:
            return "rational";
        case LEX_VECTOR:
            return "vector";
        case LEX_MATRIX:
            return "matrix";
        case LEX_PROCESS:
            return "process";
        case LEX_INFO:
            return "info";
        case LEX_ROTATE:
            return "rotate";
        case LEX_PRINT:
            return "print";
        case LEX_WRITE:
            return "write";
        case LEX_ROW:
            return "row";
        case LEX_COLUMN:
            return "column";
        case LEX_CANONICAL:
            return "make_canonical";


        case LEX_FIN:
            return "EOF";
        case LEX_COMMA:
            return ",";
        case LEX_OPEN_ROUND_BRACKET:
            return "(";
        case LEX_CLOSE_ROUND_BRACKET:
            return ")";
        case LEX_QUOT:
            return "\"";
        case LEX_SLASH:
            return "/";
        case LEX_DOT:
            return ".";
        case LEX_SHARP_COMM:
            return "#";
        case LEX_STAR:
            return "*";
        case LEX_COLON:
            return ":";
        case LEX_SEMICOLON:
            return ";";
        case LEX_OPEN_SQUARE_BRACKET:
            return "[";
        case LEX_CLOSE_SQUARE_BRACKET:
            return "]";
        case LEX_PLUS:
            return "+";
        case LEX_MINUS:
            return "-";
        case LEX_EQUAL:
            return "=";
        case LEX_POWER:
            return "^";

        case LEX_OPEN_COMM:
            return "OPEN_COMM";
        case LEX_CLOSE_COMM:
            return "CLOSE_COMM";
        case LEX_VAR:
            return "VAR";
        case LEX_STRING:
            return "STRING";
        case LEX_DIGIT:
            return "DIGIT";
        case LEX_DOUBLE:
            return "DOUBLE";
        case LEX_FUNCTION:
            return "FUNCTION";
        case LEX_CONST:
            return "CONST";
        case LEX_UNARY_MINUS:
            return "#";
        case LEX_UNARY_PLUS:
            return "$";
        case LEX_GET_ELEM:
            return "GET_ELEM";
        default:
            return "UNDEFINED";
        }
    }
    friend std::string lex_to_string(type_of_lex t)
    {
        Lex l(0, 0);
        return l.get_str_value(t);
    }

    friend std::ostream& operator << (std::ostream& s,const Lex &l)
    {
        s << "{ POS(" << l.get_line() << ", " << l.get_column() <<
                    "); TYPE = <" << l.get_str_value(l.get_type()) << ">;";
        if(l.get_value() != "")
            s << " VALUE = <" << l.get_value() << ">;";
        return s << " }";
    }
};

#endif // LEX_H
