#include<iostream>
#include<string>
#include<map>
#include<algorithm>
#include <regex>
#include<stdexcept>

using namespace std;

enum Tokens {
    NUM = 20,
    ID = 21,
    INT = 22,
    EOL = 23,
    IF = 24,
    ELSE = 25
};

class Token{
public:
    char kind;        	 // what kind of token
    int value;     	    // for numbers a value 
    string name;     	// for variables a name

    //Constructor delegation
    explicit Token(char ch) : Token(ch, 0, "") { }
    Token(char ch, int val) : Token(ch, val, "") { }
    Token(char ch, string n) : Token(ch, 0, move(n)) { }
    Token(char ch, int val, string n) : kind(ch), value(val), name(move(n)) {}
};

class Token_stream {
    bool full;              // is there a Token in the buffer?
    Token buffer;           // keep a Token when using putback()
public:

    Token_stream() : full(false), buffer(0) {}
    Token get();            // get a Token
    void putback(Token t);  // put a Token back
};

// declarations so that functions can call each other
int statement();
int declaration();
int condition();
int expression();
int term();
int primary();

//Logic operations
int logic_or();
int logic_and();
int logic_bigger_smaller();

