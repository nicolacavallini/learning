#include <functional>
#include <iostream>
#include <map>


void first_function()
{
    std::cout << "i am the first one" <<std::endl;
}

void second_function()
{
    std::cout << "i am the second one" <<std::endl;
}



int main()
{
    std::map<std::string, std::function<void()> > what_function_shall_i_call;

    what_function_shall_i_call["first"] = std::function<void()>(first_function);
    what_function_shall_i_call["second"] = std::function<void()>(second_function);

    what_function_shall_i_call["first"]();
    what_function_shall_i_call["second"]();

}
