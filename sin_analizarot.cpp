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
void Sin_analizarot::write_var_map(const string& filename) const
{
    ofstream f;
    Ident *id;
    f.open(filename);
    for(pair<string, Ident*> i : var_map)
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
void Sin_analizarot::process() {return;} //TODO
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
                    idVector = new Ident_Vector(name,
                                   Vector(stoi(dec_vector[0].get_value()),
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
        throw "Smth bad happend";
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
        curr_type = curr_lex.get_type();
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
    for(pair<string, Ident*> i : var_map)
        delete i.second;
}
