#include "declare_parser.h"
#include <iostream>
#define SIZE_OF_TW 12
#define SIZE_OF_TD 16

using namespace std;
Declare_parser::Declare_parser(const string &filename)
{
    file.open(filename);
    if(!file)
    {
        //can not open
    }
    file.unsetf(ios_base::skipws);
    line = 1;
    column = 1;
    prevline = 1;
    prevcolumn = 1;
    CS = START;
}

string Declare_parser::TW[] =
{
    "",         //0
    "declare",  //1
    "integer",  //2
    "float",    //3
    "rational", //4
    "vector",   //5
    "matrix",   //6
    "process",  //7
    "info",     //8
    "rotate",   //9
    "print",    //10
    "write"     //11
};

char Declare_parser::TD[] =
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
    '^'     //16
};

char Declare_parser::skipNS(char c)
{
    while(c == ' ' || c == '\n' || c == '\t')
    {
        if(!(file >> c)) return '\0';
        linecolumn(c);
    }
    return c;
}

void Declare_parser::linecolumn(char c)
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

type_of_lex Declare_parser::lookTD(const char& str) const
{
    for(unsigned int i = 0; i < SIZE_OF_TD; i++)
    {
        if(TD[i] == str)
        {
            if(i == 0)  return LEX_NULL;
            return static_cast<type_of_lex>(i + SIZE_OF_TW - 1);
        }
    }
    return LEX_NULL;
}
type_of_lex Declare_parser::lookTW(const string& str) const
{
    for(unsigned int i = 0; i < SIZE_OF_TW; i++)
        if(TW[i] == str)
            return static_cast<type_of_lex>(i);
    return LEX_NULL;
}
char Declare_parser::sharp_comm()
{
    char c = '#';
    while(c != '\n')
    {
        if(!(file >> c)) return '\0';
        linecolumn(c);
    }
    return c;
}
char Declare_parser::big_comm()
{
    char c1 = '/';
    char c2 = '*';
    while(!(c1 == '*' && c2 == '/'))
    {
        c1 = c2;
        if(!(file >> c2)) return '\0';
        linecolumn(c2);
    }
    return c2;
}
string Declare_parser::quotes()
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
void Declare_parser::print() const
{
    for(auto const &i : lex_vector)
        cout << i << endl;
}

void Declare_parser::write(const string &str) const
{
    ofstream f;
    f.open(str);
    for(auto const &i : lex_vector)
        f << i << endl;
    f.close();
}
bool Declare_parser::isdigit(const string & str) const
{
    for(auto const &i : str)
        if(i < '0' || i > '9')
            return false;
    return true;
}
void Declare_parser::parse()
{
    char c;
    string str = "";
    type_of_lex typeTD, typeTW;
    Lex obj(1,1);

    while(file >> c)
    {
        linecolumn(c);

        if(c == '\t' || c == '\n') c = ' ';
        if(str == "")
        {
            if((c = skipNS(c)) == '\0')
                break;
        }
        if((typeTD = lookTD(c)) != LEX_NULL || c== ' ')
        {
            if(str != "")
            {
                if((typeTW = lookTW(str)) != LEX_NULL)
                {
                    obj.set_type(typeTW);
                    obj.set_value("");
                }
                else
                {
                    if(isdigit(str))
                        obj.set_type(LEX_DIGIT);
                    else
                        obj.set_type(LEX_VAR);
                    obj.set_value(str);
                }
                lex_vector.push_back(obj);
                //obj.set_linecol(prevline, prevcolumn);
                str = "";
            }
            if(typeTD == LEX_SLASH)
            {
                //obj.set_linecol(prevline, prevcolumn);
                if(file >> c)
                {
                    linecolumn(c);
                    if((typeTD = lookTD(c)) != LEX_NULL)
                    {
                        if(typeTD == LEX_STAR)
                        {
                            obj.set(LEX_OPEN_COMM);
                            lex_vector.push_back(obj);
                            if(big_comm() == '\0')
                                break;
                            obj.set_linecol(prevline, prevcolumn - 1);
                            obj.set(LEX_CLOSE_COMM);
                            lex_vector.push_back(obj);
                            continue;
                        }
                        obj.set(LEX_SLASH);
                        lex_vector.push_back(obj);
                    }
                    else
                    {
                        obj.set(LEX_SLASH);
                        lex_vector.push_back(obj);
                        if(c == ' ') continue;
                        obj.set_linecol(prevline, prevcolumn);
                        str += c;
                        continue;
                    }
                }
                else
                    break;
            }
            if(typeTD == LEX_SHARP_COMM)
            {
                obj.set_linecol(prevline, prevcolumn);
                obj.set(LEX_SHARP_COMM);
                lex_vector.push_back(obj);
                if(sharp_comm() == '\0')
                    break;
                //obj.set_linecol(line, column);
                continue;
            }
            if(typeTD == LEX_QUOT)
            {
                obj.set_linecol(prevline, prevcolumn);
                obj.set(LEX_QUOT);
                lex_vector.push_back(obj);
                obj.set_linecol(line, column);
                try{str = quotes();}
                catch(char &c){break;}
                obj.set_value(str);
                obj.set_type(LEX_STRING);
                if(str != "")
                    lex_vector.push_back(obj);
                str = "";
                obj.set_linecol(prevline, prevcolumn);
                obj.set(LEX_QUOT);
                lex_vector.push_back(obj);
                continue;
            }
            obj.set_linecol(prevline, prevcolumn);
            obj.set(typeTD);
            if(typeTD != LEX_NULL)
                lex_vector.push_back(obj);
            str = "";
            //obj.set_linecol(line, column);
            continue;
        }
        str += c;
        if(str.length() == 1)
            obj.set_linecol(prevline, prevcolumn);
    }
    if(str != "")
    {
        obj.set_value(str);
        if(isdigit(str))
            obj.set_type(LEX_DIGIT);
        else
            obj.set_type(LEX_VAR);
        lex_vector.push_back(obj);
    }
}
