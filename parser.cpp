#include "scanner.h"
#include "symtab.h"

// globals
Token_stream ts;
Symbol_table st; 
//map<string, int> names;

int statement()
{
	Token t = ts.get();        // get the next token from token stream
	switch (t.kind) {
	case INT:
		return declaration();
    case IF:
	    return condition();
	default:
		ts.putback(t);     // put t back into the token stream
		return logic_or();
	}
}

int declaration()
{
	Token t = ts.get();
	if (t.kind != ID) throw runtime_error("name expected in declaration");
	string name = t.name;
        st.declare(name, 0);
	return 0;
}

int condition()
{
    int stipulation = logic_or(); //Read condition value
    int do_statement = logic_or();
    Token t = ts.get();
    int do_not_statement = logic_or();

    if(t.kind != ELSE){
        throw runtime_error("if without else");
    }
    else if(stipulation != 0){ //i.e. condition is true
        return do_statement;
    }
    else{
        return do_not_statement;
    }
}

// + and -
int expression()
{
    int left = term();      // read and evaluate a Term
    Token t = ts.get();        // get the next token from token stream

    while (true) {
        switch (t.kind) {
            case '+':
                left += term();    // evaluate Term and add
                t = ts.get();
                break;
            case '-':
                left -= term();    // evaluate Term and subtract
                t = ts.get();
                break;
            default:
                ts.putback(t);     // put t back into the token stream
                return left;       // finally: no more + or -: return the answer
        }
    }
}

// * and /
int term()
{
    int left = primary();
    Token t = ts.get();

    while (true) {
        switch (t.kind) {
            case '*':
                left *= primary();
                t = ts.get();
                break;
            case '/':
            {
                int d = primary();
                if (d == 0) throw runtime_error("divide by zero");
                left /= d;
                t = ts.get();
                break;
            }
            default:
                ts.putback(t);
                return left;
        }
    }
}

int primary()
{
    Token t = ts.get();
    switch (t.kind) {
        case '(':    // handle '(' expression ')'
        {
            int d = logic_or();
            t = ts.get();
            if (t.kind != ')'){
                throw runtime_error("')' expected");
            }else{
                return d;
            }
        }
        case '-':
            return - primary();
        case '+':
            return primary();
        case '!':
            return !primary();  //! has higher precedence then * and /
        case NUM:
            return t.value;  // return the number value
        case ID:
        {
            string n = t.name;
            Token next = ts.get();
            if (next.kind == '=') {	// name = expression
                    int d = logic_or();
                st.set(n, d);
                    return d; // return the assignment value
            }
            else {
                    ts.putback(next);		// not an assignment
                    return st.get(t.name).value;  // return the id value
            }
        }
        default:
            throw runtime_error("primary expected");
    }
}

int logic_or()
{
    //Logical AND has higher precedence then logical OR
    int left = logic_and();
    Token t = ts.get();

    while(true) {
        if (t.kind == '|') {
            int right = logic_and();//Every operator calls the one with higher precedence
            left = (left || right);//since we need to parse the right side anyway
            t = ts.get();
        } else {
            ts.putback(t);
            return left;
        }
    }

}
int logic_and(){
    int left = logic_bigger_smaller();
    Token t = ts.get();

    while(true) {
        if (t.kind == '&') {
            int right = logic_bigger_smaller(); //since we need to parse the right side anyway
            left = (left && right);
            t = ts.get();
        } else {
            ts.putback(t);
            return left;
        }
    }
}
int logic_bigger_smaller(){
    int left = expression();
    Token t = ts.get();

    while (true) {
        switch(t.kind){
            case '<':
                left = (left < expression());
                t = ts.get();
                break;
            case '>':
                left = (left > expression());
                t = ts.get();
                break;
            default:
                ts.putback(t);
                return left;
        }
    }
}
