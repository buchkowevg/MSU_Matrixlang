#include <iostream>
#include <fstream>
#include <string.h>
#include "mystring.h"
#include "vector.h"
#include <stdlib.h>
#include "vector_ex.h"
#include "rational_number_ex.h"
using namespace std;

#define READING_VECTOR 0
#define WAITING_FOR_VECTOR 1
#define WAITING_FOR_LENGTH 2
#define READING_LENGTH 3
#define WAITING_FOR_ENDSHARPWOV 4
#define WAITING_FOR_ENDSHARP 5
#define WAITING_FOR_KEY 6
#define WAITING_FOR_ENDL 7
#define READING_KEY 8
#define WAITING_FOR_DATA 9
#define READING_DATA 10
Vector::Vector()
{
    tree = NULL;
    number_of_elements = 0;
}
Vector::Vector(const Vector &obj)
{
    number_of_elements = obj.get_number_of_elements();
    tree = NULL;
    Rational_number rat;
    for(unsigned int i = 0; i < number_of_elements; i++)
    {
        if((rat = obj[i]) != 0)
        {
            try
            {
                tree = insert(tree, i, rat);
            }
            catch(...)
            {
                throw Vector_ex(V_ALLOC_ERR);
            }
        }
    }
}
Vector::Vector(int length)
{
    tree = NULL;
    number_of_elements = (unsigned int)length;
}
Vector::Vector(unsigned int length)
{
    tree = NULL;
    number_of_elements = length;
}
Vector::Vector(unsigned int length, Rational_number num)
{
    node *temp = NULL;
    tree = NULL;
    number_of_elements = length;
    try
    {
        if(num == 0) return;
        for(unsigned int i = 0; i < length; i++)
        {
            temp = insert(tree, i, num);
            tree = temp;
        }
    }
    catch(...)
    {
        tree = remove(tree);
        number_of_elements = 0;;
        throw Vector_ex(V_ALLOC_ERR);
    }
}
Vector::Vector(const char* file_name)
{
    ifstream inFile;
    String str;
    tree = NULL;
    char *s = NULL;
    char c;
    char buf[6];
    buf[5] = '\0';
    int r = WAITING_FOR_VECTOR;
    int count = 4;
    unsigned int elem_n = 0;
    number_of_elements = 0;
    Rational_number rat;
    inFile.open(file_name);
    if (!inFile) {
        throw Vector_ex(V_UNABLE_TO_OPEN, 0, 0 ,file_name);
        return;
    }
    inFile.unsetf(ios_base::skipws);
    try
    {
        while(inFile >> c)
        {
            if(c == '\t') c = ' ';
            if(r == WAITING_FOR_ENDSHARPWOV)
            {
               if(c == '\n')
               {
                    r = WAITING_FOR_VECTOR;
                    continue;
               }
               else continue;
            }
            if(r == WAITING_FOR_ENDSHARP)
            {
                if(c == '\n')
                {
                     r = WAITING_FOR_KEY;
                     continue;
                }
                else continue;
            }
            if(r == WAITING_FOR_VECTOR)
            {
                if(c == ' ' || c == '\n') continue;
                if(c == '#')
                {
                    r = WAITING_FOR_ENDSHARPWOV;
                    continue;
                }
                r = READING_VECTOR;
            }
            if(r == READING_VECTOR)
            {
                if(count < 0)
                {
                    if(strcmp(buf, "vecto") == 0 && c == 'r')
                    {
                        r = WAITING_FOR_LENGTH;
                        continue;
                    }
                    else
                    {
                        throw Vector_ex(V_NO_VECTOR, 0, 0 ,file_name);
                        break;
                    }
                }
                else
                {
                    buf[4 - count] = c;
                    count--;
                }
                continue;
            }
            if(r == WAITING_FOR_LENGTH)
            {
                if(c == ' ') continue;
                else
                {
                    if(c >= '0' && c <= '9') r = READING_LENGTH;
                    else
                    {
                        throw Vector_ex(V_NO_LENGTH, 0, 0 ,file_name);
                        break;
                    }
                }
            }
            if(r == READING_LENGTH)
            {
                if(c >= '0' && c <= '9') number_of_elements = c - '0' + number_of_elements * 10;
                else r = WAITING_FOR_ENDL;
            }
            if(r == WAITING_FOR_ENDL)
            {
                if(c == '#')
                {
                    r = WAITING_FOR_ENDSHARP;
                    continue;
                }
                if(c == ' ') continue;
                if(c == '\n')
                {
                    r = WAITING_FOR_KEY;
                    elem_n = 0;
                    continue;
                }
                throw Vector_ex(V_BAD_FORMAT, 0, 0 ,file_name);
                break;
            }
            if(r == WAITING_FOR_KEY)
            {
                if(c == '#')
                {
                    r = WAITING_FOR_ENDSHARP;
                    continue;
                }
                if(c == ' ' || c == '\n') continue;
                else r = READING_KEY;
            }
            if(r == READING_KEY)
            {
                if(c >= '0' && c <= '9')
                {
                    elem_n = c - '0' + elem_n * 10;
                    continue;
                }
                else
                {
                    if(elem_n == 0)
                    {
                        throw Vector_ex(V_ZERO_INDEX, 0, 0 ,file_name);
                        break;
                    }
                    if(elem_n > number_of_elements )
                    {
                        throw Vector_ex(V_F_OUT_OF_RANGE, 0, 0 ,file_name);
                        break;
                    }
                    r = WAITING_FOR_DATA;
                }
            }
            if(r == WAITING_FOR_DATA)
            {
                if(c == ' ') continue;
                else r = READING_DATA;
            }
            if(r == READING_DATA)
            {
                if(c == '\n' || c == '#')
                {
                    if(str.getlength() == 0) rat = 0;
                    else
                    {
                        s = str.getchararr();
                        rat = s;
                        free(s);
                    }
                    s = NULL;
                    if(c == '#') r = WAITING_FOR_ENDSHARP;
                    else r = WAITING_FOR_KEY;
                    str = "";
                    if(rat != 0) tree = insert(tree,elem_n - 1,rat);
                    elem_n = 0;
                    continue;
                }
                str = str + c;
            }
        }
        if(r == READING_DATA || r == WAITING_FOR_DATA)
        {
            if(str.getlength() == 0) rat = 0;
            else
            {
                s = str.getchararr();
                rat = s;
                free(s);
            }
            s = NULL;
            r = WAITING_FOR_KEY;
            if(rat != 0) tree = insert(tree,elem_n - 1,rat);
            elem_n = 0;
        }
        if(r == WAITING_FOR_LENGTH) throw Vector_ex(V_NO_LENGTH, 0, 0 ,file_name);
        if(r == WAITING_FOR_ENDSHARPWOV || r == WAITING_FOR_VECTOR || r == READING_VECTOR)
            throw Vector_ex(V_NO_VECTOR, 0, 0 ,file_name);
    }
    catch(const Vector_ex& ex)
    {
        tree = remove(tree);
        number_of_elements = 0;
        throw ex;
    }
    catch(const Rational_number_ex& ex)
    {
        tree = remove(tree);
        number_of_elements = 0;
        throw ex;
    }
    catch(...)
    {
        tree = remove(tree);
        number_of_elements = 0;
        throw Vector_ex(V_ALLOC_ERR);
    }
}
Vector::~Vector()
{
    tree = remove(tree);
    number_of_elements = 0;
}
unsigned int Vector::get_number_of_elements() const
{
    return number_of_elements;
}
Vector& Vector::operator =(const Vector& obj)
{
    tree = remove(tree);
    number_of_elements = obj.get_number_of_elements();
    Rational_number rat;
    for(unsigned int i = 0; i < number_of_elements; i++)
    {
        if((rat = obj[i]) != 0)
        {
            try
            {
                tree = insert(tree, i, rat);
            }
            catch(...)
            {
                throw Vector_ex(V_ALLOC_ERR);
            }
        }
    }
    return *this;
}
char* Vector::get_str() const
{
    String str, str1;
    try
    {
        if(number_of_elements == 0) return str.getchararr();
        for(unsigned int i = 0; i < number_of_elements; i++)
        {
            str1 = get_elem(tree ,i).get_String();
            str = str + str1;
            str = str + "\n";
        }
    }
    catch(...)
    {
        throw Vector_ex(V_ALLOC_ERR);
    }
    return str.getchararr();
}
char* Vector::get_true_str() const
{
    String str;
    Rational_number r;
    try
    {
        if(number_of_elements == 0) return str.getchararr();
        for(unsigned int i = 0; i < number_of_elements; i++)
        {
            if((r = get_elem(tree, i)) == 0) continue;
            str = str +r.get_String() + "\n";
        }
    }
    catch(...)
    {
        throw Vector_ex(V_ALLOC_ERR);
    }
    return str.getchararr();
}
Rational_number& Vector::operator ()(unsigned int key)
{
    if(key >= number_of_elements) throw Vector_ex(V_OUT_OF_RANGE, this);
    else
    {
        try
        {
            tree = insert(tree, key, 0);
        }
        catch(...)
        {
            tree = remove(tree);
            number_of_elements = 0;
            throw Vector_ex(V_ALLOC_ERR);
        }
    }
    return get_elemref(tree, key);
}
Rational_number Vector::operator [](unsigned int key) const
{
    if(key >= number_of_elements)
    {
        throw Vector_ex(V_OUT_OF_RANGE, this);
        return 0;
    }
    return get_elem(tree, key);
}
Vector operator +(const Vector& obj1, const Vector& obj2)
{
    Vector res(obj1.get_number_of_elements());
    Rational_number rat1, rat2;
    if(obj1.get_number_of_elements() != obj2.get_number_of_elements())
    {
        throw Vector_ex(V_BAD_SIZES, &obj1, &obj2);
        return 0;
    }
    for(unsigned int i = 0; i < obj1.get_number_of_elements(); i++)
    {
        if((rat1 = obj1[i]) == 0)
        {
            if((rat2 = obj2[i]) == 0) continue;
        }
        else rat2 = obj2[i];
        try
        {
            res(i) = rat1 + rat2;
        }
        catch(...)
        {
            throw Vector_ex(V_ALLOC_ERR);
        }
    }
    return res;
}
Vector Vector::operator -() const
{
    Vector res(this->number_of_elements);
    Rational_number rat;
    for(unsigned int i = 0; i < this->number_of_elements; i++)
    {
        if((rat = this->get_elem(this->tree,i)) != 0)
        {
            try
            {
                res(i) = -rat;
            }
            catch(...)
            {
                throw Vector_ex(V_ALLOC_ERR);
            }
        }
    }
    return res;
}
Vector operator -(const Vector& obj1, const Vector& obj2)
{
    return obj1 + (-obj2);
}
Rational_number operator *(const Vector& obj1, const Vector& obj2)
{
    Rational_number res = 0;
    if(obj1.get_number_of_elements() != obj2.get_number_of_elements())
    {
        throw Vector_ex(V_BAD_SIZES, &obj1, &obj2);
        return 0;
    }
    for(unsigned int i = 0; i < obj1.get_number_of_elements(); i++)
    {
        res += obj1[i] * obj2[i];
        res.make_canonical();
    }
    return res;
}
Vector operator +(const Vector& obj1, Rational_number obj2)
{
    return obj1 + Vector(obj1.get_number_of_elements(), obj2);
}
Vector operator -(const Vector& obj1, Rational_number obj2)
{
    return obj1 - Vector(obj1.get_number_of_elements(), obj2);
}
Vector operator *(const Vector& obj1, Rational_number obj2)
{
    Vector res(obj1.get_number_of_elements());
    if(obj2 == 0) return res;
    Rational_number r;
    for(unsigned int i = 0; i < obj1.get_number_of_elements(); i++)
    {
        try
        {
            if((r = obj1[i] * obj2) != 0) res(i) = r;
        }
        catch(...)
        {
            throw Vector_ex(V_ALLOC_ERR);
        }
    }
    return res;
}
Vector operator /(Vector obj1, Rational_number obj2)
{
    Vector res(obj1.get_number_of_elements());
    Rational_number r;
    for(unsigned int i = 0; i < obj1.get_number_of_elements(); i++)
    {
        try
        {
            if((r = obj1[i] / obj2) != 0) res(i) = r;
        }
        catch(Rational_number_ex* e)
        {
            throw e;
        }
        catch(...)
        {
            throw Vector_ex(V_ALLOC_ERR);
        }
    }
    return res;
}
Vector operator +(const Rational_number obj1, const Vector& obj2)
{
    return obj2 + obj1;
}
Vector operator -(const Rational_number obj1, const Vector& obj2)
{
    return (-obj2) + obj1;
}
Vector operator *(const Rational_number obj1, const Vector& obj2)
{
    return obj2 * obj1;
}
Vector operator /(const Rational_number obj1, const Vector& obj2)
{
    Vector res(obj2.get_number_of_elements());
    Rational_number r;
    for(unsigned int i = 0; i < obj2.get_number_of_elements(); i++)
    {
        try
        {
            if((r = obj2[i] / obj1) != 0) res(i) = r;
        }
        catch(Rational_number_ex* e)
        {
            throw e;
        }
        catch(...)
        {
            throw Vector_ex(V_ALLOC_ERR);
        }
    }
    return res;
}
char* to_string(const Vector &obj1)
{
    return obj1.get_str();
}
void Vector::write(const char *filename, bool rot) const
{
    ofstream myfile;
    char* s;
    char c = '\n';
    Rational_number rat;
    if(rot)
        c = ' ';
    myfile.open(filename);
    if(myfile.is_open())
    {
        myfile << "vector " << number_of_elements << '\n';
        for(unsigned int i = 0; i < number_of_elements; i++)
        {
            if((rat = get_elem(tree, i)) != 0)
            {
                s = rat.get_str();
                myfile << i + 1 << ' ' << s << c;
                free(s);
            }
        }
    }
    else throw Vector_ex(V_UNABLE_TO_OPEN, 0, 0, filename);
}

//tree functions
unsigned char Vector::height(node* p) const
{
    return p?p->height:0;
}
int Vector::bfactor(node* p) const
{
    return height(p->right)-height(p->left);
}
void Vector::fixheight(node* p)
{
    unsigned char hl = height(p->left);
    unsigned char hr = height(p->right);
    p->height = (hl>hr?hl:hr)+1;
}
node* Vector::rotateright(node* p) // правый поворот вокруг p
{
    node* q = p->left;
    p->left = q->right;
    q->right = p;
    fixheight(p);
    fixheight(q);
    return q;
}
node* Vector::rotateleft(node* q) // левый поворот вокруг q
{
    node* p = q->right;
    q->right = p->left;
    p->left = q;
    fixheight(q);
    fixheight(p);
    return p;
}
node* Vector::balance(node* p) // балансировка узла p
{
    fixheight(p);
    if( bfactor(p)==2 )
    {
        if( bfactor(p->right) < 0 )
            p->right = rotateright(p->right);
        return rotateleft(p);
    }
    if( bfactor(p)==-2 )
    {
        if( bfactor(p->left) > 0  )
            p->left = rotateleft(p->left);
        return rotateright(p);
    }
    return p; // балансировка не нужна
}
Rational_number Vector::get_elem(node *p, int k) const
{
    if(p == NULL) return 0;
    if( k < p->key ) return get_elem(p->left,k);
    else
    {
        if(k > p->key) return get_elem(p->right,k);
        else return p->data;
    }
}
Rational_number& Vector::get_elemref(node *p, int k)
{
    if( k < p->key ) return get_elemref(p->left,k);
    else
    {
        if(k > p->key) return get_elemref(p->right,k);
        else return p->data;
    }
}
node* Vector::insert(node* p, int k, Rational_number d) // вставка ключа k в дерево с корнем p
{
    if( p == NULL ) return new node(k, d);
    if( k < p->key )
        p->left = insert(p->left,k, d);
    else
    {
        if(k > p->key)p->right = insert(p->right,k, d);
        else p->data = d;
    }
    return balance(p);
}
node* Vector::findmin(node* p) // поиск узла с минимальным ключом в дереве p
{
    return p->left?findmin(p->left):p;
}

node* Vector::removemin(node* p) // удаление узла с минимальным ключом из дерева p
{
    if( p->left==NULL )
        return p->right;
    p->left = removemin(p->left);
    return balance(p);
}

node* Vector::remove(node* p, int k) // удаление ключа k из дерева p
{
    if( p == NULL ) return NULL;
    if( k < p->key )
        p->left = remove(p->left,k);
    else if( k > p->key )
        p->right = remove(p->right,k);
    else //  k == p->key
    {
        node* q = p->left;
        node* r = p->right;
        delete p;
        if( !r ) return q;
        node* min = findmin(r);
        min->right = removemin(r);
        min->left = q;
        return balance(min);
    }
    return balance(p);
}
node* Vector::remove(node *p)
{
    if(p == NULL) return NULL;
    if(p->right != NULL) remove(p->right);
    if(p->left != NULL) remove(p->left);
    delete p;
    return NULL;
}
Vector Vector::make_canonical()
{
    Rational_number rat;
    for(unsigned int i = 0; i < number_of_elements; i++)
        if((rat = this->operator [](i)) != 0)
            this->operator ()(i) = rat.make_canonical();
    return *this;
}
std::ostream& operator << (std::ostream& s, const Vector &v)
{
    char* c = v.get_str();
    s << c;
    free(c);
    return s;
}


