#include "sin_analizarot.h"

#include <string.h>
#include <iostream>
#include <fstream>
#include "../sparse/sparse_ex.h"
using namespace std;

string lex_to_string(type_of_lex);
Sin_analizarot::Sin_analizarot(const std::vector<Lex> &l, const std::string &f)
{
    lex_vector = l;
    filename = f;
}
void Sin_analizarot::gl()
{
    if(lex_vector.empty())
        throw "lex_vector empty";
    curr_lex = lex_vector.front();
    lex_vector.erase(lex_vector.begin());
}
void Sin_analizarot::write_poliz(const string &filename) const
{
    ofstream f;
    f.open(filename);
    for(auto& i : poliz_vector)
    {
        switch (i->get_type()) {
        case LEX_VAR:
            f << "'<" + i->get_value()->get_name() + ">' ";
            break;
        case LEX_CONST:
            switch(static_cast<Poliz_Const*>(i)->get_value()->get_type())
            {
            case LEX_STRING:
                f << "'\"" + static_cast<Ident_String*>(static_cast<Poliz_Const*>(i)->get_value())->get_value() +
                     "\"' ";
                break;
            case LEX_INTEGER:
                f << "'" + to_string(static_cast<Ident_Int*>(static_cast<Poliz_Const*>(i)->get_value())->get_value()) +
                     "' ";
                break;
            case LEX_FLOAT:
                f << "'" + to_string(static_cast<Ident_Float*>(static_cast<Poliz_Const*>(i)->get_value())->get_value()) +
                     "' ";
                break;
            default:
                f << "<UNDEFINED CONST> ";
                break;
            }
            break;
        case LEX_FUNCTION:
            if(static_cast<Poliz_Function*>(i)->get_oper() != LEX_SEMICOLON)
                f << lex_to_string(static_cast<Poliz_Function*>(i)->get_oper()) << " ";
            else
                f << ";\n";
            break;
        default:
            f << "<UNDEFINED> ";
            break;
        }
    }
    f.close();
}
void Sin_analizarot::write_var_map(const string& filename) const
{
    ofstream f;
    Ident *id;
    f.open(filename);
    for(auto& i : var_map)
    {
        id = i.second;
        switch (id->get_type())
        {
        case LEX_INTEGER:
            f << "***********" << endl;
            f << "TYPE = <" << lex_to_string(id->get_type()) << "> NAME = <" << id->get_name() << ">" << endl;
            f << static_cast<Ident_Int*>(id)->get_value() << endl;
            break;
        case LEX_FLOAT:
            f << "***********" << endl;
            f << "TYPE = <" << lex_to_string(id->get_type()) << "> NAME = <" << id->get_name() << ">" << endl;
            f << static_cast<Ident_Float*>(id)->get_value() << endl;
            break;
        case LEX_RATIONAL:
            f << "***********" << endl;
            f << "TYPE = <" << lex_to_string(id->get_type()) << "> NAME = <" << id->get_name() << ">" << endl;
            f << static_cast<Ident_Rational*>(id)->get_value() << endl;
            break;
        case LEX_VECTOR:
            f << "***********" << endl;
            f << "TYPE = <" << lex_to_string(id->get_type()) << "> NAME = <" << id->get_name() << ">" << endl;
            f << static_cast<Ident_Vector*>(id)->get_value() << endl;
            break;
        case LEX_MATRIX:
            f << "***********" << endl;
            f << "TYPE = <" << lex_to_string(id->get_type()) << "> NAME = <" << id->get_name() << ">" << endl;
            f << static_cast<Ident_Matrix*>(id)->get_value() << endl;
            break;
        default:
            f << "UNDEFINED";
            break;
        }
    }
    f.close();
}
void Sin_analizarot::exeption(const std::string &str) const
{
    throw filename + ":" + to_string(curr_lex.get_line()) + ":" +
            to_string(curr_lex.get_column()) + ": error: " + str;
}
bool Sin_analizarot::isoper(type_of_lex type) const
{
    switch (type) {
    case LEX_STAR:
        return true;
    case LEX_SLASH:
        return true;
    case LEX_PLUS:
        return true;
    case LEX_MINUS:
        return true;
    case LEX_POWER:
        return true;
    case LEX_EQUAL:
        return true;
    default:
        return false;
    }
}
bool Sin_analizarot::isvar_const(type_of_lex type) const
{
    switch (type) {
    case LEX_DIGIT:
        return true;
    case LEX_DOUBLE:
        return true;
    case LEX_VAR:
        return true;
    case LEX_STRING:
        return true;
    default:
        return false;
    }
}
bool Sin_analizarot::isfunction(type_of_lex type) const
{
    switch (type) {
    case LEX_CANONICAL:
        return true;
    case LEX_ROTATE:
        return true;
    case LEX_PRINT:
        return true;
    case LEX_WRITE:
        return true;
    case LEX_ROW:
        return true;
    case LEX_COLUMN:
        return true;
    case LEX_READ:
        return true;
    default:
        return false;
    }
}
unsigned int Sin_analizarot::check_your_privilege(type_of_lex type) const
{
    if(isfunction(type))
        return 8;
    switch (type) {
    case LEX_SEMICOLON:
        return 11;
    case LEX_OPEN_SQUARE_BRACKET:
        return 10;
    case LEX_CLOSE_SQUARE_BRACKET:
        return 10;
    case LEX_UNARY_MINUS:
        return 9;
    case LEX_COLON:
        return 7;
    case LEX_POWER:
        return 6;
    case LEX_SLASH:
        return 5;
    case LEX_STAR:
        return 5;
    case LEX_PLUS:
        return 4;
    case LEX_MINUS:
        return 4;
    case LEX_EQUAL:
        return 3;
    case LEX_OPEN_ROUND_BRACKET:
        return 2;
    case LEX_CLOSE_ROUND_BRACKET:
        return 2;
    case LEX_COMMA:
        return 1;
    default:
        return 0;
    }
}
void Sin_analizarot::add_to_poliz()
{
    Lex l;
    if(curr_lex.get_type() == LEX_UNARY_PLUS)
        return;
    if(check_your_privilege(curr_lex.get_type()) == 0)
    {
        if(curr_lex.get_type() == LEX_VAR)
        {
            if(var_map.find(curr_lex.get_value()) == var_map.end())
                exeption("'" + curr_lex.get_value() + "' was not declared");
            poliz_vector.push_back(new Poliz_Var(var_map.at(curr_lex.get_value()), curr_lex));
        }
        else
        {
            switch (curr_lex.get_type()) {
            case LEX_DIGIT:
                poliz_vector.push_back(new Poliz_Const(
                                           new Ident_Int("",
                                                         stoi(curr_lex.get_value()),
                                                         curr_lex.get_line(), curr_lex.get_column()),
                                           curr_lex));
                break;
            case LEX_DOUBLE:
                poliz_vector.push_back(new Poliz_Const(
                                           new Ident_Float("",
                                                           stod(curr_lex.get_value()),
                                                           curr_lex.get_line(), curr_lex.get_column()),
                                           curr_lex));
                break;
            case LEX_STRING:
                poliz_vector.push_back(new Poliz_Const(
                                           new Ident_String("",
                                                           curr_lex.get_value(),
                                                           curr_lex.get_line(), curr_lex.get_column()),
                                           curr_lex));
                break;
            default:
                throw "Smth bad happend in add_to_poliz";
                break;
            }
        }
    }
    else
    {
        if(curr_lex.get_type() == LEX_OPEN_SQUARE_BRACKET || curr_lex.get_type() == LEX_OPEN_ROUND_BRACKET)
            poliz_stack.push(curr_lex);
        else if (curr_lex.get_type() == LEX_SEMICOLON)
        {
            while(!poliz_stack.empty())
            {
                poliz_vector.push_back(new Poliz_Function(poliz_stack.top().get_type(), poliz_stack.top()));
                poliz_stack.pop();
            }
            poliz_vector.push_back(new Poliz_Function(LEX_SEMICOLON, curr_lex));
        }
        else if(poliz_stack.empty())
            poliz_stack.push(curr_lex);
        else if(curr_lex.get_type() == LEX_CLOSE_ROUND_BRACKET)
        {
            while((l = poliz_stack.top()).get_type() != LEX_OPEN_ROUND_BRACKET)
            {
                poliz_vector.push_back(new Poliz_Function(l.get_type(), l));
                poliz_stack.pop();
            }
            poliz_stack.pop();
        }
        else if(curr_lex.get_type() == LEX_CLOSE_SQUARE_BRACKET)
        {
            while((l = poliz_stack.top()).get_type() != LEX_OPEN_SQUARE_BRACKET)
            {
                poliz_vector.push_back(new Poliz_Function(l.get_type(), l));
                poliz_stack.pop();
            }
            poliz_stack.pop();
            l.set_type(LEX_GET_ELEM);
            poliz_vector.push_back(new Poliz_Function(l.get_type(), l));
        }
        else
        {
            if((l = poliz_stack.top()).get_type() == LEX_OPEN_ROUND_BRACKET ||
                    l.get_type() == LEX_OPEN_SQUARE_BRACKET)
                poliz_stack.push(curr_lex);
            else
            {
                if(curr_lex.get_type() == LEX_UNARY_MINUS)
                {
                    if(check_your_privilege(l.get_type()) > check_your_privilege(curr_lex.get_type()))
                    {
                        poliz_vector.push_back(new Poliz_Function(l.get_type(), l));
                        poliz_stack.pop();
                        poliz_stack.push(curr_lex);
                    }
                    else
                    {
                        poliz_stack.push(curr_lex);
                    }

                }
                else if(check_your_privilege(l.get_type()) >= check_your_privilege(curr_lex.get_type()))
                {
                    poliz_vector.push_back(new Poliz_Function(l.get_type(), l));
                    poliz_stack.pop();
                    poliz_stack.push(curr_lex);
                }
                else
                {
                    poliz_stack.push(curr_lex);
                }
            }
        }
    }
}

void Sin_analizarot::info()
{
    while(curr_lex.get_type() == LEX_INFO)
    {
        gl();
        info();
        if(curr_lex.get_type() != LEX_OPEN_ROUND_BRACKET)
            exeption("expected '(' got '" + lex_to_string(curr_lex.get_type()) + "'");
        gl();
        info();
        if(curr_lex.get_type() != LEX_STRING)
            exeption("expected 'const string' got '" + lex_to_string(curr_lex.get_type()) + "'");
        gl();
        info();
        if(curr_lex.get_type() != LEX_CLOSE_ROUND_BRACKET)
            exeption("expected ')' got '" + lex_to_string(curr_lex.get_type()) + "'");
        gl();
    }

}
void Sin_analizarot::unary()
{
    while(curr_lex.get_type() == LEX_PLUS || curr_lex.get_type() == LEX_MINUS)
    {
        if(curr_lex.get_type() == LEX_PLUS)
        {
            curr_lex.set_type(LEX_UNARY_PLUS);
            add_to_poliz();
        }
        else
        {
            curr_lex.set_type(LEX_UNARY_MINUS);
            add_to_poliz();
        }
        gl();
        info();
    }
    info();

}
void Sin_analizarot::function()
{
    info();
    if(!isfunction(curr_lex.get_type()))
        exeption("expected 'function' got '" + lex_to_string(curr_lex.get_type()));
    add_to_poliz();
    if(curr_lex.get_type() == LEX_WRITE ||
            curr_lex.get_type() == LEX_READ)
    {
        gl();
        info();
        if(curr_lex.get_type() != LEX_OPEN_ROUND_BRACKET)
            exeption("expected '(' got '" + lex_to_string(curr_lex.get_type()) + "'");
        gl();
        info();
        if(curr_lex.get_type() != LEX_STRING)
            exeption("expected 'const string' got '" + lex_to_string(curr_lex.get_type()) + "'");
        add_to_poliz();
        gl();
        info();
        if(curr_lex.get_type() != LEX_CLOSE_ROUND_BRACKET)
            exeption("expected ')' got '" + lex_to_string(curr_lex.get_type()) + "'");
    }
    else if(curr_lex.get_type() == LEX_ROW || curr_lex.get_type() == LEX_COLUMN)
    {
        Lex open_br;
        gl();
        info();
        if(curr_lex.get_type() != LEX_OPEN_ROUND_BRACKET)
            exeption("expected '(' got '" + lex_to_string(curr_lex.get_type()) + "'");
        open_br = curr_lex;
        gl();
        info();
        bracket();
        if(curr_lex.get_type() != LEX_CLOSE_ROUND_BRACKET)
        {
            curr_lex = open_br;
            exeption("unterminated '('");
        }
    }
    gl();
    info();
}
void Sin_analizarot::square_oper()
{

    info();
    if(curr_lex.get_type() == LEX_COLON)
    {
        gl();
        function();
        square_oper();
    }
    else if(curr_lex.get_type() == LEX_CLOSE_SQUARE_BRACKET)
        return;
    else if(isoper(curr_lex.get_type())
            || curr_lex.get_type() == LEX_COMMA)
    {
        add_to_poliz();
        gl();
    }
    else if(curr_lex.get_type() == LEX_OPEN_SQUARE_BRACKET)
    {
        Lex open_b = curr_lex;
        add_to_poliz();
        gl();
        info();
        square_bracket();
        if(curr_lex.get_type() != LEX_CLOSE_SQUARE_BRACKET)
        {
            curr_lex = open_b;
            exeption("unterminated '['");
        }
        add_to_poliz();
        gl();
        info();
    }
    else
        exeption("expected 'operator' got '" + lex_to_string(curr_lex.get_type()) +
                 "'");
}
void Sin_analizarot::square_bracket()
{
    if(curr_lex.get_type() == LEX_CLOSE_SQUARE_BRACKET)
        exeption("empty '[]' operator");
    while(curr_lex.get_type() != LEX_CLOSE_SQUARE_BRACKET &&
          curr_lex.get_type() != LEX_FIN)
    {
        info();
        unary();
        if(curr_lex.get_type() == LEX_OPEN_ROUND_BRACKET)
        {
            Lex open_b = curr_lex;
            add_to_poliz();
            gl();
            info();
            bracket();
            if(curr_lex.get_type() != LEX_CLOSE_ROUND_BRACKET)
            {
                curr_lex = open_b;
                exeption("unterminated '('");
            }
            add_to_poliz();
            gl();
            info();
        }
        else
        {
            if(!isvar_const(curr_lex.get_type()))
                exeption("expected 'var' or 'const' got '" + lex_to_string(curr_lex.get_type()));
            add_to_poliz();
            gl();
            info();
        }
        square_oper();
    }
}
void Sin_analizarot::bracket_oper()
{
    info();
    if(curr_lex.get_type() == LEX_COLON)
    {
        gl();
        function();
        bracket_oper();
    }
    else if(curr_lex.get_type() == LEX_CLOSE_ROUND_BRACKET)
        return;
    else if(isoper(curr_lex.get_type()))
    {
        add_to_poliz();
        gl();
    }
    else if(curr_lex.get_type() == LEX_OPEN_SQUARE_BRACKET)
    {
        Lex open_b = curr_lex;
        add_to_poliz();
        gl();
        info();
        square_bracket();
        if(curr_lex.get_type() != LEX_CLOSE_SQUARE_BRACKET)
        {
            curr_lex = open_b;
            exeption("unterminated '['");
        }
        add_to_poliz();
        gl();
        info();
    }
    else
        exeption("expected 'operator' got '" + lex_to_string(curr_lex.get_type()) +
                 "'");
}
void Sin_analizarot::bracket()
{
    if(curr_lex.get_type() == LEX_CLOSE_ROUND_BRACKET)
        exeption("empty '()'");
    Lex open_br;
    while(curr_lex.get_type() != LEX_CLOSE_ROUND_BRACKET &&
          curr_lex.get_type() != LEX_FIN)
    {
        info();
        unary();
        if(curr_lex.get_type() == LEX_OPEN_ROUND_BRACKET)
        {
            open_br = curr_lex;
            add_to_poliz();
            gl();
            bracket();
            if(curr_lex.get_type() != LEX_CLOSE_ROUND_BRACKET)
            {
                curr_lex = open_br;
                exeption("unterminated '('");
            }
            add_to_poliz();
            gl();
            info();
        }
        else
        {
            if(!isvar_const(curr_lex.get_type()))
                exeption("expected 'var' or 'const' got '" + lex_to_string(curr_lex.get_type()));
            add_to_poliz();
            gl();
        }
        bracket_oper();
    }
}
void Sin_analizarot::process_oper()
{
    info();
    if(curr_lex.get_type() == LEX_COLON)
    {
        gl();
        function();
        process_oper();
    }
    else if(isoper(curr_lex.get_type()) ||
            curr_lex.get_type() == LEX_SEMICOLON)
    {
        add_to_poliz();
        gl();
    }
    else if(curr_lex.get_type() == LEX_OPEN_SQUARE_BRACKET)
    {
        Lex open_b = curr_lex;
        add_to_poliz();
        gl();
        info();
        square_bracket();
        if(curr_lex.get_type() != LEX_CLOSE_SQUARE_BRACKET)
        {
            curr_lex = open_b;
            exeption("unterminated '['");
        }
        add_to_poliz();
        gl();
        info();
        process_oper();
    }
    else
        exeption("expected 'operator' got '" + lex_to_string(curr_lex.get_type()) +
                 "'");
}
void Sin_analizarot::process2()
{
    while(curr_lex.get_type() != LEX_FIN &&
          curr_lex.get_type() != LEX_SEMICOLON)
    {
        Lex open_br;
        info();
        if(curr_lex.get_type() == LEX_SEMICOLON)
        {
            add_to_poliz();
            gl();
        }
        else
        {
            unary();
            if(curr_lex.get_type() == LEX_OPEN_ROUND_BRACKET)
            {
                add_to_poliz();
                open_br = curr_lex;
                gl();
                bracket();
                if(curr_lex.get_type() != LEX_CLOSE_ROUND_BRACKET)
                {
                    curr_lex = open_br;
                    exeption("unterminated '('");
                }
                add_to_poliz();
                gl();
                info();
                process_oper();
            }
            else
            {
                if(!isvar_const(curr_lex.get_type()))
                    exeption("expected 'var' or 'const' got '" + lex_to_string(curr_lex.get_type())
                             + "'");
                add_to_poliz();
                gl();
                process_oper();
            }
        }
    }
}
void Sin_analizarot::process1()
{
    while(curr_lex.get_type() != LEX_FIN)
    {
        while(curr_lex.get_type() == LEX_SEMICOLON)
            gl();
        process2();
    }
}
void Sin_analizarot::process()
{
    if(curr_lex.get_type() == LEX_FIN)
        return;
    process1();
}
void Sin_analizarot:: constructor1()
{
    string str = "";
    unsigned int line = 0;
    unsigned int column = 0;
    type_of_lex type = LEX_NULL;
    while(true)
    {
        line = curr_lex.get_line();
        column = curr_lex.get_column();
        type = LEX_NULL;
        str = "";
        if(curr_lex.get_type() == LEX_MINUS)
        {
            gl();
            switch (curr_lex.get_type())
            {
            case LEX_DOUBLE:
                str += '-';
                str += curr_lex.get_value();
                type = LEX_FLOAT;
                gl();
                break;
            case LEX_DIGIT:
                str = "-";
                str = curr_lex.get_value();
                type = LEX_DIGIT;
                gl();
                if(curr_lex.get_type() == LEX_SLASH)
                {
                    str += '/';
                    gl();
                    if(curr_lex.get_type() == LEX_MINUS)
                    {
                        str += '-';
                        gl();
                    }
                    if(curr_lex.get_type() != LEX_DIGIT)
                        exeption(string("unexpected '") + lex_to_string(curr_lex.get_type()) +
                                 "' expected digit");
                    str += curr_lex.get_value();
                    type = LEX_RATIONAL;
                    gl();
                }
                break;
            default:
                exeption(string("unexpected '") + lex_to_string(curr_lex.get_type()) +
                         "' expected digit or float");
                break;
            }
        }
        else
        {
            if(curr_lex.get_type() != LEX_DIGIT && curr_lex.get_type() != LEX_DOUBLE &&
                    curr_lex.get_type() != LEX_STRING)
                exeption(string("unexpected '") + lex_to_string(curr_lex.get_type()) +
                         "' expected digit or string or float");
            if(curr_lex.get_type() == LEX_DIGIT)
            {
                str += curr_lex.get_value();
                type = LEX_DIGIT;
                gl();
                if(curr_lex.get_type() == LEX_SLASH)
                {
                    str += '/';
                    gl();
                    if(curr_lex.get_type() == LEX_MINUS)
                    {
                        str += '-';
                        gl();
                    }
                    if(curr_lex.get_type() != LEX_DIGIT)
                        exeption(string("unexpected '") + lex_to_string(curr_lex.get_type()) +
                                 "' expected digit");
                    type = LEX_RATIONAL;
                    str += curr_lex.get_value();
                    gl();
                }
            }
            else if(curr_lex.get_type() == LEX_DOUBLE)
            {
                type = LEX_FLOAT;
                str += curr_lex.get_value();
                gl();
            }
            else if (curr_lex.get_type() == LEX_STRING)
            {
                type = LEX_STRING;
                str += curr_lex.get_value();
                gl();
            }
        }
        dec_vector.emplace_back(line, column, type, str);
        if(curr_lex.get_type() == LEX_CLOSE_ROUND_BRACKET)
            return;
        if(curr_lex.get_type() != LEX_COMMA)
            exeption(string("unexpected '") + lex_to_string(curr_lex.get_type()) +
                     "' expected ',' or ')");
        gl();
    }
}
void Sin_analizarot::constructor()
{
    if(curr_lex.get_type() == LEX_CLOSE_ROUND_BRACKET)
        return;
    constructor1();
    gl();
}
string Sin_analizarot::get_dec_str() const
{
    string ex = "";
    for(const auto &i : dec_vector)
    {
        ex += lex_to_string(i.get_type());
        ex += ", ";
    }
    ex[ex.length() - 2] = ')';
    return ex;
}
void Sin_analizarot::push_dec(string &name, unsigned int l, unsigned int c)
{
    Ident_Int *idInt = nullptr;
    Ident_Float *idFLoat = nullptr;
    Ident_Rational *idRational = nullptr;
    Ident_Vector *idVector = nullptr;
    Ident_Matrix *idMatrix = nullptr;

    switch (curr_type) {
    case LEX_INTEGER:
        if(dec_vector.size() == 0)
            idInt = new Ident_Int(name, 0, l, c);
        else
        {
            if(dec_vector[0].get_type() != LEX_DIGIT && dec_vector.size() != 1)
                exeption("there is no such constructor for int (" + get_dec_str());
            idInt = new Ident_Int(name, stoi(dec_vector[0].get_value()), l, c);
        }
        var_map.insert(pair<string, Ident*>(name, idInt));
        break;
    case LEX_FLOAT:
        if(dec_vector.size() == 0)
            idFLoat = new Ident_Float(name, 0, l, c);
        else
        {
            if(dec_vector[0].get_type() != LEX_FLOAT || dec_vector.size() != 1)
                exeption("there is no such constructor for float (" + get_dec_str());
            idFLoat = new Ident_Float(name, stod(dec_vector[0].get_value()), l, c);
        }
        var_map.insert(pair<string, Ident*>(name, idFLoat));
        break;
    case LEX_RATIONAL:
        switch (dec_vector.size())
        {
        case 0:
            idRational = new Ident_Rational(name, 0, l, c);
            break;
        case 1:
            if(dec_vector[0].get_type() == LEX_DIGIT)
                idRational = new Ident_Rational(name, dec_vector[0].get_value().c_str(), l, c);
            else if(dec_vector[0].get_type() == LEX_STRING ||
                    dec_vector[0].get_type() == LEX_RATIONAL)
            {
                try
                {
                    idRational = new Ident_Rational(name, dec_vector[0].get_value().c_str(), l ,c);
                }
                catch(const Sparse_ex &ex)
                {
                    exeption(string("rational constructor error ") + ex.what());
                }
            }
            else
                exeption("there is no such constructor for rational (" + get_dec_str());
            break;
        case 2:
            if((dec_vector[0].get_type() == LEX_DIGIT && dec_vector[1].get_type() == LEX_DIGIT) ||
                    (dec_vector[0].get_type() == LEX_STRING && dec_vector[1].get_type() == LEX_STRING))
            {
                try
                {
                    idRational = new Ident_Rational(name, Rational_number(dec_vector[0].get_value().c_str(),
                                                    dec_vector[1].get_value().c_str()), l ,c);
                }
                catch(const Sparse_ex &ex)
                {
                    exeption(string("rational constructor error ") + ex.what());
                }
            }
            else
                exeption("there is no such constructor for rational (" + get_dec_str());

            break;
        default:
            exeption("there is no such constructor for rational (" + get_dec_str());
            break;
        }
        var_map.insert(pair<string, Ident*>(name, idRational));
        break;
    case LEX_VECTOR:
        switch (dec_vector.size())
        {
        case 0:
            idVector = new Ident_Vector(name, Vector(0), l, c);
            break;
        case 1:
            if(dec_vector[0].get_type() == LEX_DIGIT)
                idVector = new Ident_Vector(name, Vector(stoi(dec_vector[0].get_value())), l, c);
            else if(dec_vector[0].get_type() == LEX_STRING)
            {
                try
                {
                    idVector = new Ident_Vector(name, Vector(dec_vector[0].get_value().c_str()), l, c);
                }
                catch(const Sparse_ex &ex)
                {
                    exeption(string("vector constructor error ") + ex.what());
                }
            }
            else
                exeption("there is no such constructor for vector (" + get_dec_str());
            break;
        case 2:
            if(dec_vector[0].get_type() == LEX_DIGIT &&
                    (dec_vector[1].get_type() == LEX_DIGIT ||
                     dec_vector[1].get_type() == LEX_RATIONAL))
            {
                try
                {
                    idVector = new Ident_Vector(name, Vector(
                                   stoi(dec_vector[0].get_value()),
                                      Rational_number(dec_vector[1].get_value().c_str())), l, c);
                }
                catch(Sparse_ex &ex)
                {
                    exeption(string("vector constructor error ") + ex.what());
                }
            }
            else
                exeption("there is no such constructor for vector (" + get_dec_str());
            break;
        default:
            exeption("there is no such constructor for vector (" + get_dec_str());
            break;
        }
        var_map.insert(pair<string, Ident*>(name, idVector));
        break;
    case LEX_MATRIX:
        switch (dec_vector.size())
        {
        case 0:
            idMatrix = new Ident_Matrix(name, Matrix((unsigned int)0), l, c);
            break;
        case 1:
            if(dec_vector[0].get_type() == LEX_DIGIT)
                idMatrix = new Ident_Matrix(name, Matrix(stoi(dec_vector[0].get_value())), l, c);
            else if(dec_vector[0].get_type() == LEX_STRING)
            {
                try
                {
                    idMatrix = new Ident_Matrix(name, Matrix(dec_vector[0].get_value().c_str()), l, c);
                }
                catch(const Sparse_ex &ex)
                {
                    exeption(string("matrix constructor error ") + ex.what());
                }
            }
            else
                exeption("there is no such constructor for matrix (" + get_dec_str());
            break;
        case 2:
            if(dec_vector[0].get_type() == LEX_DIGIT)
            {
                if(dec_vector[1].get_type() == LEX_RATIONAL)
                {
                    try
                    {
                        idMatrix = new Ident_Matrix(name,
                                       Matrix(stoi(dec_vector[0].get_value()),
                                             Rational_number(dec_vector[1].get_value().c_str())), l, c);
                    }
                    catch(const Sparse_ex &ex)
                    {
                        exeption(string("matrix constructor error ") + ex.what());
                    }
                }
                else if(dec_vector[1].get_type() == LEX_DIGIT)
                {
                    try
                    {
                        idMatrix = new Ident_Matrix(name,
                                   Matrix(stoi(dec_vector[0].get_value()),
                                                    stoi(dec_vector[1].get_value())), l, c);
                    }
                    catch(const Sparse_ex &ex)
                    {
                        exeption(string("matrix constructor error ") + ex.what());
                    }

                }
                else
                    exeption("there is no such constructor for matrix (" + get_dec_str());
            }
            else
                exeption("there is no such constructor for matrix (" + get_dec_str());
            break;
        case 3:
            if(dec_vector[0].get_type() == LEX_DIGIT)
            {
                if(dec_vector[1].get_type() == LEX_DIGIT)
                {
                    if(dec_vector[2].get_type() == LEX_RATIONAL)
                    {
                        try
                        {
                            idMatrix = new Ident_Matrix(name,
                                    Matrix(stoi(dec_vector[0].get_value()), stoi(dec_vector[1].get_value()),
                                      Rational_number(dec_vector[2].get_value().c_str())), l, c);
                        }
                        catch(const Sparse_ex &ex)
                        {
                            exeption(string("matrix constructor error ") + ex.what());
                        }
                    }
                    else
                        exeption("there is no such constructor for matrix (" + get_dec_str());
                }
                else if(dec_vector[1].get_type() == LEX_RATIONAL)
                {
                    if(dec_vector[2].get_type() == LEX_DIGIT)
                    {
                        try
                        {
                            idMatrix = new Ident_Matrix(name, Matrix(stoi(dec_vector[0].get_value()), Rational_number(dec_vector[1].get_value().c_str()), bool(stoi(dec_vector[2].get_value()))), l, c);
                        }
                        catch(const Sparse_ex &ex)
                        {
                            exeption(string("matrix constructor error ") + ex.what());
                        }
                    }
                    else
                        exeption("there is no such constructor for matrix (" + get_dec_str());
                }
                else
                    exeption("there is no such constructor for matrix (" + get_dec_str());
            }
            else
                exeption("there is no such constructor for matrix (" + get_dec_str());
            break;
        default:
            exeption("there is no such constructor for matrix (" + get_dec_str());
            break;
        }
        var_map.insert(pair<string, Ident*>(name, idMatrix));
        break;
    default:
        throw "Smth bad happend in push_dec";
        break;
    }
    dec_vector.clear();
}
void Sin_analizarot::declaration()
{
    string name = "";
    Ident *id = nullptr;
    unsigned int line = 0;
    unsigned int column = 0;
    while(true)
    {
        if(curr_lex.get_type() == LEX_SEMICOLON)
        {
            gl();
            return;
        }
        else
        {
            if(curr_lex.get_type() != LEX_VAR)
                exeption(string("unexpected '") +
                         lex_to_string(curr_lex.get_type()) +
                         "' expected name");
            line = curr_lex.get_line();
            column = curr_lex.get_column();
            name = curr_lex.get_value();
            if(var_map.find(name) != var_map.end())
            {
                id = var_map.find(name)->second;
                exeption(string("redeclaration of '") + name + "' previously declared as "
                         + lex_to_string(id->get_type()) + ":" + to_string(id->get_line()) +
                         ":" + to_string(id->get_column()) + ":");
            }
            gl();
            if(curr_lex.get_type() == LEX_OPEN_ROUND_BRACKET)
            {
                gl();
                constructor();
            }
            push_dec(name, line, column);
            if(curr_lex.get_type() != LEX_COMMA && curr_lex.get_type() != LEX_SEMICOLON)
                exeption(string("unexpected '") +
                         lex_to_string(curr_lex.get_type()) +
                         "' expected ',' or ';'");
            if(curr_lex.get_type() == LEX_COMMA)
                gl();
        }
    }
}
void Sin_analizarot::declare()
{
    while(true)
    {
        while(curr_lex.get_type() == LEX_SEMICOLON)
            gl();
        curr_type = curr_lex.get_type();
        if(curr_type != LEX_INTEGER && curr_type != LEX_FLOAT
                && curr_type != LEX_RATIONAL && curr_type != LEX_VECTOR
                && curr_type != LEX_MATRIX && curr_type != LEX_PROCESS)
            exeption(string("unexpected '") +
                     lex_to_string(curr_lex.get_type()) +
                     "' expected 'integer' or 'float' or 'rational' or 'vector'" +
                     " or 'matrix' or 'process");
        //curr_type = curr_lex.get_type();
        gl();
        if(curr_lex.get_type() != LEX_COLON)
            exeption(string("unexpected '") +
                     lex_to_string(curr_lex.get_type()) + "' expected ':'");
        gl();
        if(curr_type == LEX_PROCESS)
        {
            process();
            return;
        }
        declaration();
    }
}
void Sin_analizarot::parse()
{
    gl();
    if(curr_lex.get_type() == LEX_DECLARE)
    {
        gl();
        if(curr_lex.get_type() != LEX_COLON)
            exeption(string("unexpected '") + lex_to_string(curr_lex.get_type()) + "' expected ':'");
        gl();
        declare();
    }
    else
    {
        if(curr_lex.get_type() != LEX_PROCESS)
            exeption(string("unexpected '") + lex_to_string(curr_lex.get_type()) + "' expected 'process'");
        gl();
        if(curr_lex.get_type() != LEX_COLON)
            exeption(string("unexpected '") + lex_to_string(curr_lex.get_type()) + "' expected ':'");
        gl();
        process();
    }
}
Sin_analizarot::~Sin_analizarot()
{
    for(auto& i : var_map)
        delete i.second;
    for(auto& i : poliz_vector)
        delete i;
}
