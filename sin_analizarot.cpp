#include "sin_analizarot.h"

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
void Sin_analizarot::process() {} //TODO
void Sin_analizarot::declaration()
{
    if(curr_lex.get_type() == LEX_SEMICOLON)
    {
        gl();
        declare();
    }
    else
    {
        if(curr_lex.get_type() != LEX_VAR)
            throw "UNEXPECTED SMTH";
        if(var_map.find(curr_lex.get_value()) == var_map.end())
            throw "REDECALRATION";
        //PUSH ETC
        gl();
    }

}
void Sin_analizarot::declare()
{
    while(curr_lex.get_type() == LEX_SEMICOLON)
        gl();
    curr_type = curr_lex.get_type();
    if(curr_type != LEX_INTEGER || curr_type != LEX_FLOAT
            || curr_type != LEX_RATIONAL || curr_type != LEX_VECTOR
            || curr_type != LEX_MATRIX || curr_type != LEX_PROCESS)
        throw "UNEXPECTED SMTH";
    gl();
    if(curr_lex.get_type() != LEX_COLON)
        throw "EXPECTED :";
    gl();
    if(curr_type == LEX_PROCESS)
        process();
    else
        declaration();
}
void Sin_analizarot::parse()
{
    gl();
    if(curr_lex.get_type() == LEX_PROCESS)
    {
        gl();
        if(curr_lex.get_type() != LEX_COLON)
            throw "EXPECTED :";
        gl();
        process();
    }
    else
    {
        if(curr_lex.get_type() != LEX_DECLARE)
            throw "NO PROCESS";
        gl();
        if(curr_lex.get_type() != LEX_COLON)
            throw "EXPECTED :";
        gl();
        declare();
    }
}
