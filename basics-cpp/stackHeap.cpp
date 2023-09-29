#include <iostream>

int main()
{
    int val = 5;         // Save val data in STACK
    std::cout << val << " From the STACK\n";
    int* hval = new int; // Save hval data in HEAP
    *hval = 5;
    std::cout << hval[0] << " From the HEAP\n";

    int a[5];
    a[3] = 10;            // Array on the STACK
    std::cout << a[3] << " From a STACK array\n";
    int* ha = new int[5]; // Array on the HEAP
    ha[3] = 13;
    std::cout << ha[3] << " From a HEAP array\n";
    
    delete hval;          // Must DELETE memory from HEAP
    delete ha;

    // Stack Memory
    //      - Predefined size of memory (a few MB)
    //      - Very easy to run out of stack.
    //      - without the 'new' call, local variable 
    //        are stored on stack by  default.
    //      - "Static" memory, memory requirements must
    //        be known at COMPILE TIME.

    // Heap Memory
    //      - Much more space available.
    //      - Allows for dynamic memory.
    //      - Allocated via 'new' keyword.
    //          Type * varName = new Type;
    //      - Calls C function malloc under the hood.
    //      - OS finds a contiguous chunk of memory and returns
    //        pointer to it. This is VERY EXPENSIVE.

    // RAII
    return 0;
}