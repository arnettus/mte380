#include "../../libraries/RollingAverage/RollingAverage.h"
#include <assert.h>
#include <stdio.h>

int main() {
    RollingAverage<uint64_t> ra(3);

    printf("\nTesting the RollingAverage class:\n\n");

    ra.add(3);
    ra.add(3);
    ra.print();
    printf("Expected average: 3\n\n");
    assert(ra.read() == 3);

    ra.add(3);
    ra.print(); 
    printf("Expected average: 3\n\n");
    assert(ra.read() == 3);

    ra.add(0);
    ra.print();
    printf("Expected average: 2\n\n");
    assert(ra.read() == 2);

    ra.add(0);
    ra.print();
    printf("Expected average: 1\n\n");
    assert(ra.read() == 1);

    ra.reset(); 
    ra.print();
    printf("Expected average: 0\n\n");
    assert(ra.read() == 0);

    ra.add(3);
    ra.add(3);
    ra.add(3);
    ra.add(0);
    ra.print();
    printf("Expected average: 2\n\n");
    assert(ra.read() == 2);

    ra.reset(5);
    ra.print();
    printf("Expected average: 0\n\n");
    assert(ra.read() == 0);

    ra.add(3);
    ra.add(3);
    ra.add(3);
    ra.add(0);
    ra.add(0);
    ra.print();
    printf("Expected average: 2\n\n");
    assert(ra.read() == 2);

    printf("..Success!\n\n");

    return 0;
}
