#include <iostream>

void psi(int & i)
{
    // Print stack information of integer input.
    // Must pass pointer (*) or reference (&) in, otherwise 
    // input ints are simply copied into the same memory address 
    // over and over.
    // REMEMBER: functions will by default COPY the input vars.
    std::cout << &i << " " << i << " " << sizeof(i) << "\n";
}

// Use template to make the arrays truely dyanmic!
template <class T>
class DynamicArray
{
    size_t m_size; // need to initialize size, C++ only knows where
                   // where arrays begin, not how long they are.
    T *  m_arr;
public:
    // Constructor acquires the resource.
    DynamicArray(size_t size)
        : m_size(size)
        , m_arr (new T[size]{})
    {
        // Do something whenever the array is constructed.
        std::cout << "Array Constructor\n";
    }
    // Destructor
    ~DynamicArray()
    {
        delete [] m_arr;
        std::cout << "Array Destructor\n";
    }

    T get(size_t index) const
    {
        // takes an index and outputs array value 
        // (cannot modify output array val so use const)
        return m_arr[index];
    }

    void set(size_t index, T val)
    {
        // takes index and value andd sets arr element
        // not const since we are modifying
        m_arr[index] = val;
    }

    void print() const
    {
        // standard printing procedure for array
        for (size_t i=0; i<m_size; i++)
        {
            std::cout << i << " " << m_arr[i] << "\n";
        }
    }
    // Operator Overloading
    // Hijack [] operator to allow for dynamic value setting
    const T & operator [] (size_t index) const
    {
        return m_arr[index];
    }
    T & operator [] (size_t index)
    {
        return m_arr[index];
    }
};

int main(int argc, char * argv[])
{
    std::cout << "Stack Allocated Memory\n";
    int a = 10;
    int b = 25;

    std::cout << "Single ints:\n" "Address   Value   Size\n";
    psi(a);
    psi(b);

    std::cout << "\nContiguous Array:\n" "Address   Value   Size\n";
    int arr[5] = {}; // = {} gives default values (0) for array.
    for (size_t i=0; i<10; i++)
    {
        psi(arr[i]);
    }

    int* pa = &a;
    int* pb = &b;

    *pa = 17;
    // Equivaluent to *(&a) = 17;
    std::cout << "Modify variable by accessing its pointer\n";
    psi(a);

    // We can also modify values by doing pointer math

    *(pb-1) = 25;
    // Equivalent to *pa = 25;

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

    // RAII (Resource Acquisition is Initialization)
        // Use class construction to build and maintain dynamic arrays
        // Array stored in HEAP and auto releases memory on destruction.
    
    DynamicArray<int> myArray(5); // constructs
    
    myArray.set(2, 134);
    myArray.set(4, 12);
    myArray.print();

    // RAII compliant class allows for dynamic typing!
    DynamicArray<float> myFArray(5);
    myFArray.set(3, 12.5);
    myFArray.get(3);
    myFArray.print();

    // But what if I want to jjust set an array element on the fly?
    // Operator Overloading.
    myFArray[3] = 13.37;
    myFArray.print();
    // Pointers. Why Use Them?
    //      1. Must use pointer for inheritance (Base * ptr = new Derived();)
    //      2. Pass by value vs. by reference
    //      3. Pointing to large data
    return 0;
}