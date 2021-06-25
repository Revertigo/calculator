#include<iostream>
#include<string>
#include<map>
#include<algorithm>
using namespace std;

class Attributes {
public:
    int value;
    explicit Attributes(int nv) : value(nv) { }
};

class Symbol_table {
public:
    Attributes& get(const string & name);                 // return the attributes of name
    void set(const string & name, int nv);                // set the attribute of value
    bool is_declared(const string & name);                // is name already in symtab?
    void declare(const string & name, int nv);            // add name to symtab
private:
    map<string, Attributes> sym_tab;
};

