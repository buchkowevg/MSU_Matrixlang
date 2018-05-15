#include "mystring.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cmath>
using namespace std;

char* fr(char* ch)
{
    if(ch != NULL)
    {
        free(ch);
        ch = NULL;
    }
    return NULL;
}

String::String(const char *inp)
{
    length = 0;
    str = NULL;
    state = 0;
    while(inp[length] != '\0') length++;
    if((str = (char*)calloc(length + 1, sizeof(char))) == NULL)
    {
        throw "Bad alloc";
        return;
    }
    for(int i = 0; i < length; i++) str[i] = inp[i];
}
String::String(char inp)
{
   length = 1;
   str = NULL;
   state = 0;
   if((str = (char*)calloc(length + 1, sizeof(char))) == NULL)
   {
       throw "Bad alloc";
       return;
   }
   str[0] = inp;
}

char& String::operator[](int index)
{
    if(state != 0)
    {
        throw "Bad alloc";
    }
    if(index >= length || index < 0)
    {
        cout << "bad index" << endl;
        return str[0];
    }
    return str[index];
}

String::String()
{
	str = NULL;
	length = 0;
	state = 0;
}

String::~String()
{
        if(str != NULL) free(str);
}

char* String::getchararr() const
{
	char* s = NULL;
    if(state != 0) return NULL;
    if((s = (char*)calloc(length + 1, sizeof(char))) == NULL)  throw "Bad alloc";
	for(int i = 0; i < length; i++) s[i] = str[i];
	return s;
}

void String::checkstr()
{
        if(str != NULL)
	{
		free(str);
		str = NULL;
	}
	length = 0;
	state = 0;
}
void String::setstring()
{
	char c = 0;
	char *check = NULL;
	checkstr();
	while((c = getchar()) != '\n')
	{
		length++;
		if((check = (char*)realloc(str, length * sizeof(char))) == NULL)
		{
            free(str);
            throw "Bad alloc";
			str = NULL;
			state = 1;
		}
		str = check;
		str[length - 1] = c;
	}
	if((check = (char*)realloc(str, (length + 1) * sizeof(char))) == NULL)
	{
		free(str);
        throw "Bad alloc";
		str = NULL;
		state = 1;
	}
    str = check;
	str[length] = '\0';
}
void String::outputstr() const
{
	if(state != 0) return;
	for(int i = 0; i < length; i++) cout << str[i];
    cout << endl;
}
void String::setstring(long long int inp)
{
	char *check = NULL; 
	checkstr();
	if((length = snprintf(NULL, 0, "%lld", inp)) < 0)
	{
		state = 2;
		length = -state;
        throw "Bad alloc";
		return;
	}
    if((str = (char*)calloc(length + 1, sizeof(char))) == NULL)
    {
    	state = 1;
		length = -state;
        return;
    }
    check = str;
    if(sprintf(str, "%lld", inp) < 0)
    {
    	free(check);
    	str = NULL;
    	state = 2;
		length = -state;
        throw "Bad alloc";
    	return;
    }
}
void String::setstring(double inp)
{
	int buffsize = 20, l;
    char buf[buffsize];
	char *check = NULL;
	double sv = 0;
	checkstr();
	if((length = snprintf(NULL, 0, "%d", int(inp))) < 0)
	{
		state = 2;
		length = -state;
        throw "Bad alloc";
		return;
	}
	sv = abs(inp - int(inp));
	if((l = sprintf(buf, "%.14f", sv)) < 0)
	{
		state = 2;
		length = -state;
        throw "Bad alloc";
		return;
	}
	l--;
	while(buf[l] == '0') l--;
	if(buf[l] == '.') l++;
	l++;
	length += l;
	if((str = (char*)calloc(length, sizeof(char))) == NULL)
	{
		state = 1;
		length = -state;
        throw "Bad alloc";
		return;
	}
	check = str;
	if(snprintf(check, length, "%.14f", inp) < 0)
	{
		free(str);
		str = NULL;
		state = 2;
		length = -state;
        throw "Bad alloc";
		return;
	}
	str = check;
	length--;
}
void String::setstring(const String &obj)
{
    checkstr();
	if((state = obj.getstate()) != 0)
	{
		length = -state;
        throw "Bad alloc";
		return;
    }
	if((str = obj.getchararr()) == NULL)
	{
		state = 1;
		length = -state;
        throw "Bad alloc";
		return;
	}
	length = obj.getlength();
}
String& String::operator =(const String &obj)

//str2.setstring(str1);
{
    checkstr();
    if(obj.getstate() != 0)
    {
        length = -obj.getstate();
        state = obj.getstate();
        str = NULL;
        throw "Bad alloc";
        return *this;
    }
    if((str = obj.getchararr()) == NULL)
    {
        state = 1;
        length = -state;
        throw "Bad alloc";
        return *this;
    }
    length = obj.getlength();
    state = obj.getstate();
    return *this;
}
String String::operator +(const String &obj) const
{
    String res;
    if(state != 0)
    {
        res = *this;
        throw "Bad alloc";
        return res;
    }
    if(obj.getstate() != 0)
    {
        res = obj;
        throw "Bad alloc";
        return res;
    }
    res.length = length + obj.getlength();
    if((res.str = (char*)calloc(res.getlength()+1, sizeof(char))) == NULL)
    {
        res.state = 1;
        res.length = -res.getstate();
        throw "Bad alloc";
        return res;
    }
    for(int i = 0; i < length; i++) res.str[i] = str[i];
    for(int i = length; i < res.length; i++) res.str[i] = obj.str[i - length];
    res.state = 0;
    return res;
}

