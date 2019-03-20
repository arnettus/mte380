#include "../../libraries/Stack/Stack.h"
#include <assert.h>
#include <stdio.h>

int main() {
    Stack<int> intStk(2);
    Stack<bool> boolStk(2);

    intStk.push(3);
    assert(intStk.pop() == 3);

    boolStk.push(true);
    assert(boolStk.pop() == true);

    boolStk.push(false);
    boolStk.push(true);
    assert(boolStk.pop() == true);

    // Instead of catching any exception I should probably throw and catch specific ones, oh well.
    boolStk.push(false);
    bool overflowExceptionThrown = false;
    try {
        boolStk.push(true);
    }
    catch(...) {
        overflowExceptionThrown = true;
    }
    assert(overflowExceptionThrown);

    bool underflowExceptionThrown = false;
    try {
        intStk.pop();
    }
    catch(...) {
        underflowExceptionThrown = true;
    }
    assert(underflowExceptionThrown);

    return 0;
}
