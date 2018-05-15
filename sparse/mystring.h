#ifndef MYSTRING_H
#define MYSTRING_H
class String
{
public:
	String();
    String(const char *inp);
    String(const String &obj) { str = 0; setstring(obj); }
    String(long long int inp) { str = 0; setstring(inp); }
    String(double inp) { str = 0; setstring(inp); }
    String(char inp);
	~String();
    unsigned int getlength() const { return length; }
	int getstate() const { return state; }
	char* getchararr() const;
    const char* _getchararr() const { return str;}
	void outputstr() const;
	void setstring();
	void setstring(const String &obj);
	void setstring(long long int inp);
	void setstring(double inp);    
    String& operator=(const String&);
    String operator+(const String&) const;
    char& operator[] (int indx);
private:
	void checkstr();
	int length;
	char *str;
    char err;
	int state;//0 - ok; 1 - alloc err; 2 - sprintf err;
};
#endif
