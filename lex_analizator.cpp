#include "lex_analizator.h"
#include <iostream>
#define SIZE_OF_TW 15
#define SIZE_OF_TD 18

using namespace std;


Lex_analizator::Lex_analizator()
{
    line = 1;
    column = 1;
    prevline = 1;
    prevcolumn = 1;
}

string Lex_analizator::TW[] =
{
    "",             //0
    "declare",      //1
    "integer",      //2
    "float",        //3
    "rational",     //4
    "vector",       //5
    "matrix",       //6
    "process",      //7
    "info",         //8
    "rotate",       //9
    "print",        //10
    "write",        //11
    "row",          //12
    "column",       //13
    "make_canonical"//14
};

char Lex_analizator::TD[] =
{
    '\0',   //0
    ',',    //1
    '(',    //2
    ')',    //3
    '"',    //4
    '/',    //5
    '.',    //6
    '#',    //7
    '*',    //8
    ':',    //9
    ';',    //10
    '[',    //11
    ']',    //12
    '+',    //13
    '-',    //14
    '=',    //15
    '^',    //16
    ' '     //17
};

char Lex_analizator::skipNS(char c)
{
    while(c == ' ' || c == '\n' || c == '\t')
    {
        if((c = gc()) == '\0')
            return '\0';
    }
    if(c == '#')
        c = skipNS(sharp_comm());
    return c;
}

void Lex_analizator::linecolumn(char c)
{
    prevline = line;
    prevcolumn = column;
    if(c == '\n')
    {
        ++line;
        column = 1;
    }
    else ++column;
}
char Lex_analizator::gc()
{
    char c;
    if(!(file >> c))
            return '\0';
    linecolumn(c);
    if(c == ' ' || c == '\n' || c == '\t')
        c = ' ';
    return c;
}
void Lex_analizator::ungc(char c)
{
    file.putback(c);
    line = prevline;
    column = prevcolumn;
}
type_of_lex Lex_analizator::lookTD(const char& str) const
{
    for(unsigned int i = 0; i < SIZE_OF_TD; i++)
        if(TD[i] == str)
            return static_cast<type_of_lex>(i + SIZE_OF_TW);
    return LEX_NULL;
}
type_of_lex Lex_analizator::lookTW(const string& str) const
{
    for(unsigned int i = 0; i < SIZE_OF_TW; i++)
        if(TW[i] == str)
            return static_cast<type_of_lex>(i);
    return LEX_NULL;
}
char Lex_analizator::sharp_comm()
{
    char c = '#';
    while(c != '\n')
        if(!(file >> c))
            return '\0';
        else
            linecolumn(c);
    return c;
}
char Lex_analizator::big_comm()
{
    char c1 = '/';
    char c2 = '*';
    while(!(c1 == '*' && c2 == '/'))
    {
        c1 = c2;
        if((c2 = gc()) == '\0') return '\0';
    }
    return c2;
}
string Lex_analizator::quotes()
{
    char c1 = '\0';
    char c2 = '\0';
    string str = "";
    while(file >> c2)
    {
        linecolumn(c2);
        if(c1 == '\\')
        {
            if(c2 == 'n')
            {
                str+='\n';
                c1 = c2;
                continue;
            }
            else
            {
                str+= c2;
                c1 = c2;
                continue;
            }
        }
        if(c2 == '\\')
        {
            c1 = c2;
            continue;
        }
        if(c2 == '"')
            return str;
        str += c2;
        c1 = c2;
    }
    throw c2;
}
void Lex_analizator::print() const
{
    for(auto const &i : lex_vector)
        cout << i << endl;
}

void Lex_analizator::write(const string &str) const
{
    ofstream f;
    f.open(str);
    for(auto const &i : lex_vector)
        f << i << endl;
    f.close();
}
bool Lex_analizator::isdigit(char c) const
{
    if(c < '0' || c > '9')
        return false;
    return true;
}
const vector<Lex>& Lex_analizator::get_vector() const
{
    return lex_vector;
}
void Lex_analizator::open(const string &f)
{
    filename = f;
    file.open(f);
    if(!file)
        throw "File \"" + f + "\" does not exist";
    file.unsetf(ios_base::skipws);
}
void Lex_analizator::parse()
{
    char c;
    string str = "";
    type_of_lex typeTD, typeTW;
    Lex obj(1,1);
    enum state{START, SLASH, STRING, DIGIT, VAR, FLOAT};
    state curr_state = START;
    c = skipNS(gc());
    while(true)
    {
        switch (curr_state) {
        case START:
            c = skipNS(c);
            obj.set_linecol(prevline, prevcolumn);
            if((typeTD = lookTD(c)) == LEX_NULL)
            {
                if(isdigit(c))
                    curr_state = DIGIT;
                else
                    curr_state = VAR;
            }
            else if(typeTD == LEX_SLASH)
                curr_state = SLASH;
            else if(typeTD == LEX_QUOT)
                curr_state = STRING;
            else if(typeTD == LEX_DOT)
                throw filename + ":" + to_string(prevline) + ":" + to_string(prevcolumn) + ": error: unexpected '.'";
            else if(typeTD == LEX_FIN)
            {
                obj.set(LEX_FIN);
                lex_vector.push_back(obj);
                return;//END OF ANALIZATOR
            }
            else
            {
                obj.set(typeTD);
                lex_vector.push_back(obj);
                c = gc();
            }
            break;
        case SLASH:
            c = gc();
            if(c == '*')
            {
                if(big_comm() == '\0')
                    throw filename + ":" + to_string(obj.get_line()) + ":" +
                        to_string(obj.get_column()) + ": error: unterminated comment";
                c = gc();
            }
            else
            {
                obj.set(LEX_SLASH);
                lex_vector.push_back(obj);
            }
            curr_state = START;
            break;
        case STRING:
            obj.set_linecol(line, column);
            try { str = quotes(); }
            catch(char &)
            { throw filename + ":" + to_string(obj.get_line()) + ":" +
                        to_string(obj.get_column()) + ": error: missing terminating \" character"; }
            obj.set(LEX_STRING, str);
            lex_vector.push_back(obj);
            c = gc();
            str = "";
            curr_state = START;
            break;
        case VAR:
            if((typeTD = lookTD(c)) != LEX_NULL)
            {
                obj.set_value(str);
                if((typeTW = lookTW(str)) != LEX_NULL)
                    obj.set(typeTW);
                else
                    obj.set_type(LEX_VAR);
                lex_vector.push_back(obj);
                str = "";
                curr_state = START;
            }
            else
            {
                str += c;
                c = gc();
                curr_state = VAR;
            }
            break;
        case DIGIT:
            if((typeTD = lookTD(c)) != LEX_NULL)
            {
                if(typeTD == LEX_DOT)
                {
                    str += c;
                    c = gc();
                    curr_state = FLOAT;
                }
                else
                {
                    obj.set(LEX_DIGIT, str);
                    lex_vector.push_back(obj);
                    str = "";
                    curr_state = START;
                }
            }
            else
            {
                if(isdigit(c))
                    str += c;
                else
                    throw filename + ":" + to_string(prevline) + ":" +
                        to_string(prevcolumn) + ": error: unexpected '" + c + "'";
                c = gc();
                curr_state = DIGIT;
            }
            break;
        case FLOAT:
            if((typeTD = lookTD(c)) != LEX_NULL)
            {
                if(typeTD == LEX_DOT)
                    throw filename + ":" + to_string(prevline) + ":" +
                        to_string(prevcolumn) + ": error: unexpected '.'";
                else
                {
                    obj.set(LEX_DOUBLE, str);
                    lex_vector.push_back(obj);
                    str = "";
                    curr_state = START;
                }
            }
            else
            {
                if(isdigit(c))
                    str += c;
                else
                    throw filename + ":" + to_string(prevline) + ":" +
                        to_string(prevcolumn) + ": error: unexpected '" + c + "'";
                c = gc();
                curr_state = FLOAT;
            }
            break;
        default:
            cout << "SMTH BAD HAPPEND" << endl;
            break;
        }
    }
}
