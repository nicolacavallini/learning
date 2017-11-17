#include <functional>
#include <iostream>
 
void f(int& n1, int& n2, const int& n3)
{
    std::cout << "In function: " << std::endl;
    std::cout <<   "n1 = " << n1 <<
                 ", n2 = " << n2 <<
                 ", n3 = " << n3 << std::endl;
    ++n1; // increments the copy of n1 stored in the function object
    ++n2; // increments the main()'s n2
    // ++n3; // compile error
}
 
int main()
{
    int n1 = 1, n2 = 2, n3 = 3;
    std::function<void()> bound_f = std::bind(f, n1, std::ref(n2), std::ref(n3));
    n1 = 10;
    n2 = 11;
    n3 = 12;
    std::cout << "Before function: " << std::endl;
    std::cout <<   "n1 = " << n1 <<  
                 ", n2 = " << n2 << 
                 ", n3 = " << n3 << std::endl;
    bound_f();
    std::cout << "After function: " << std::endl;
    std::cout <<   "n1 = " << n1 <<          
                 ", n2 = " << n2 << 
                 ", n3 = " << n3 << std::endl;

}
