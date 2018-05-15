#ifndef VECTOR_H
#define VECTOR_H
#include "rational_number.h"
#include <stdlib.h> 
struct node // структура для представления узлов дерева
{
    int key;
    unsigned char height;
    Rational_number data;
    node* left;
    node* right;
    node(int k, Rational_number d)
    { key = k; left = right = 0; height = 1; data = d;}
};
//typedef struct node node;

class Vector
{
public:
    Vector();
    Vector(const Vector& obj);
    Vector(const char*);
    Vector(int);
    Vector(unsigned int);
    Vector(unsigned int, const Rational_number);
    ~Vector();
    Vector& operator =(const Vector& obj);
    Rational_number& operator() (unsigned int);
    Rational_number operator[] (unsigned int) const;
    Vector operator -() const;
    Vector make_canonical();
    friend Vector operator +(const Vector&, const Vector&);
    friend Vector operator -(const Vector&, const Vector&);
    friend Rational_number operator *(const Vector&, const Vector&);
    friend Vector operator +(const Vector&, const Rational_number);
    friend Vector operator -(const Vector&, const Rational_number);
    friend Vector operator *(const Vector&, const Rational_number);
    friend Vector operator /(const Vector&, const Rational_number);
    friend Vector operator +(const Rational_number, const Vector&);
    friend Vector operator -(const Rational_number, const Vector&);
    friend Vector operator *(const Rational_number, const Vector&);
    friend Vector operator /(const Rational_number, const Vector&);    
    friend std::ostream& operator << (std::ostream&, const Vector&);
    friend char* to_string(const Vector&);
    char* get_str() const;
    char* get_true_str() const;
    unsigned int get_number_of_elements() const;
    void write(const char*, bool = 0) const;
private:
    node* tree;
    unsigned int number_of_elements;

    //tree functions
    unsigned char height(node* p) const;
    int bfactor(node* p) const;
    void fixheight(node *p);
    Rational_number& get_elemref(node *p, int k);
    Rational_number get_elem(node *p, int k) const;
    node* rotateright(node* p);
    node* rotateleft(node* q);
    node* balance(node* p);
    node* insert(node* p, int k, Rational_number d);
    node* findmin(node* p);
    node* removemin(node* p);
    node* remove(node* p, int k);
    node* remove(node* p);
};

#endif // VECTOR_H
