#include "scanner.h"
extern Token_stream ts;
#include <iostream>

int main(void) {
    try {
        int val = 0;
        cout << "> "; // prompt
        while (cin) {
            Token t = ts.get();
            if (t.kind == 'q') {
                cout << "quit\n";
                break;    // quit calc
            }
            if (t.kind == EOL) {         // print result
                cout << "= " << val << '\n';
                cout << "> "; // prompt
            } else {
                ts.putback(t);
                val = statement();
            }
        }
    }
    catch (exception& e) {
        cerr << "Error, " << e.what() << '\n';
        return 1;
    }
}