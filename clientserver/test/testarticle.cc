#include <iostream>
#include "article.h"

using std::cout;

int main(void) {
    Article a1("a1", "Bob", "Hello world, from Bob!");

    if(a1.title != "a1" || a1.author != "Bob" || a1.contents != "Hello world, from Bob!") {
        cout << "Article a1 variables don't match constructor values\n";
        return 1;
    }

    if(a1 != "a1" || a1 == "A1") {
        cout << "Operator overloading unsuccessfull\n";
        return 1;
    }

    cout << "All article tests passed!\n";

    return 0;
}