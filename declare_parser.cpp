#include "declare_parser.h"

#define TW_NULL      0
#define TW_DECLARE   1
#define TW_INTEGER   2
#define TW_FLOAT     3
#define TW_RATIONAL  4
#define TW_VECTOR    5
#define TW_MATRIX    6
#define TW_PROCESS   7

#define TD_NULL          0
#define TD_COMMA         1
#define TD_OPEN_BRACKET  2
#define TD_CLOSE_BRACKET 3
#define TD_BACKSLASH     4
#define TD_SLASH         5
#define TD_DOT           6
#define TD_OPEN_COMM     7
#define TD_CLOSE_COMM    8
#define TD_SHARP_COMM    9

using namespace std;
Declare_parser::Declare_parser(const string &filename)
{
    file.open(filename);
    if(!file)
    {
        //can not open
    }
    file.unsetf(ios_base::skipws);
    line = 0;
    column = 0;
    CS = START;
}

string Declare_parser::TW[] =
{
    "",          //TW_NULL
    "declare:",  //TW_DECLARE
    "integer:",  //TW_INTEGER
    "float:",    //TW_FLOAT
    "rational:", //TW_RATIONAL
    "vector:",   //TW_VECTOR
    "matrix:",   //TW_MATRIX
    "process:",  //TW_PROCESS
    nullptr
};

string Declare_parser::TD[] =
{
    "",     //TD_NULL
    ",",    //TD_COMMA
    "(",    //TD_OPEN_BRACKET
    ")",    //TD_CLOSE_BRACKET
    "\"",   //TD_BACKSLASH
    "/",    //TD_SLASH
    ".",    //TD_DOT
    "/*",   //TD_OPEN_COMM
    "*/"    //TD_CLOSE_COMM
    "#",    //TD_SHARP_COMM
    nullptr
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
    if(c == '\n')
    {
        ++line;
        column = 0;
    }
    else ++column;
}

unsigned int Declare_parser::look(const string& str, const string* voc) const
{
    unsigned int i = 0;
    while(voc[i])
    {
        if(voc[i] == str) return i;
        ++i;
    }
    return 0;
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
    while(c1 == '*' && c2 == '/')
    {
        c1 = c2;
        if(!(file >> c2)) return '\0';
        linecolumn(c2);
    }
    return c2;
}
File_state& Declare_parser::parse()
{
    char c;
    string str = "";
    unsigned int l;
    try
    {
        while(file >> c)
        {
            linecolumn(c);
            if(c == '\t') c = ' ';
            if(CS == START)
            {
                if((c = skipNS(c)) == '\0')
                {
                    //NO "PROCESS:" FOUND
                    break;
                }
                if((str += c).length() > 9)
                {
                    //NO "PROCESS" FOUND
                }
                l = look(str, TD);
                if(l == TD_SHARP_COMM)
                {
                    if((c = sharp_comm()) == '\0')
                    {
                        //NO "PROCESS" FOUND
                    }
                    str = "";
                    continue;
                }
                if(l == TD_OPEN_COMM)
                {
                    if((c = big_comm()) == '\0')
                    {
                        //NO "PROCESS" FOUND
                    }
                    str = "";
                    continue;
                }
                l = look(str, TW);
                if(l == TW_DECLARE)
                {
                    CS = DEC;
                    str = "";
                    continue;
                }

            }
        }
    }
    catch(...)
    {

    }
}
