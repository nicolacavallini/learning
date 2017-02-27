#include <iostream>
#include <functional>
#include <map>

using namespace std;

void write_ciao(){
    cout << "ciao" << endl;
}

void write_hello(){
    cout << "hello" << endl;
}

map<string,function<void()>> map_funcs_to_strings(){

    map<string,function<void()>> string_to_func;
    string_to_func["ciao"] = write_ciao;
    string_to_func["hello"] = write_hello;

    return string_to_func;
}


int main(){

    map<string,function<void()>> what_to_write = map_funcs_to_strings();

    what_to_write["ciao"]();
    what_to_write["hello"]();

    return 0;
}
