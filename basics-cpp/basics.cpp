#include <iostream> // std library
#include <vector>
#include <fstream> // file stream


class Student {
    // private member variables, all data class could get/need
    std::string m_first = "First";
    std::string m_last  = "Last";
    int         m_id    = 0;
    float       m_avg   = 0;

public:
    // public constructors andd functions
    Student() {} // Default constructor

    // Specific constructor
    // NOTE when passing in string, entire string will be COPIED
    // to avoid this, use const reference (&)
    Student(const std::string& first, const std::string& last, int id, float avg)
        : m_first   (first)
        , m_last    (last)
        , m_id      (id)
        , m_avg     (avg)
    {
    }
    int getAvg() const
    {
        return m_avg;
    }
    int getID() const
    {
        return m_id;
    }
    std::string getFirst() const
    {
        return m_first;
    }
    std::string getLast() const
    {
        return m_last;
    }

    void print() const // const prevents this function from changing
    {                  // must be present for all public functions if 
                       // the class is assigned anywhere to a const var
                       // and wishes to use the function.
                       // "const correctness."
        std::cout << m_first << " " << m_last << " ";
        std::cout << m_id << " " << m_avg << "\n";
    }
};

class Course {
    std::string m_name = "Course";
    std::vector<Student> m_students;

public:
    Course() {}
    Course(const std::string& name)
        : m_name(name)
    {
    }

    // givent a Student class, add it to vector
    void addStudent(const Student& s)
    {
        m_students.push_back(s);
    }

    // function to get reference to vector
    const std::vector<Student>& getStudents() const
    // returned obj cannot be changed       // function will not CHANGE the class
    {
        return m_students;
    }

    void loadFromFile(const std::string& filename)
    {
        // fin reads 1 string token at a time
        std::ifstream fin(filename);
        std::string first, last;
        int id;
        float avg;

        while (fin >> first)
        {
            // funnel consecutive tokens into appropriate types
            fin >> last >> id >> avg;

            // construct Student and add it to THIS Course class
            addStudent(Student(first, last, id, avg));
        }
    }

    void print() const
    {
        for (const auto& s : m_students)
        {
            s.print(); // use public void print of Student class!
        }
    }
};

// int main(){} is the core of any c++ file

int main(int argc, char * argv[]){

    // print string to console. << act as pipes.
    std::cout << "Hello, World!" << std::endl;
    
    // initiate a vector of floats and push 2 into (back to front)
    std::vector<int> vec;
    vec.push_back(42.0);
    vec.push_back(10.0);

    // use for loop to print out elements of vector
    // size_t type enforces long-unsigned integer type, avoiding some errors
    std::cout << "For loop print vector elements:\n";
    for (size_t i=0; i<vec.size(); i++){
        std::cout << vec[i] << "\n";
    }

    // range-based for loop. auto type intuitively types a.
    // WARNING: auto COPIES element from vector.
    std::cout << "AGAIN but range-based for loop:\n";
    for (auto a : vec){
        std::cout << a << "\n";
    }

    std::cout << "Create Student class objects and print:\n";
    // create class object and call public functions
    Student s1; // a student class with no attributes assigned

    const Student s2("Ashley", "Lian", 1, 3.14); // const Student class
    const Student s3("Renna", "Lian", 92329, 100.0);
    std::cout << s2.getLast() << "\n"; // call public function
    s2.print(); // call the void function

    std::cout << "Create Course class, add Students, print:\n";
    Course seansCourse("Sean's Course");
    seansCourse.addStudent(s1);
    seansCourse.addStudent(s2);
    seansCourse.addStudent(s3);

    seansCourse.print();

    std::cout << "\nPut it all together, load students.txt file, ";
    std::cout << "construct students, add to course, print: \n";
    Course c("CXX 101");
    c.loadFromFile("students.txt");
    c.print();
    
    return 0; // always return 0
}