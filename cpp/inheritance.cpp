#include <iostream>

using namespace std;

class M{
public:
    void print_protected(){
        cout << protected_data << endl;
    }
    void print_modified_protected(string add_on){
        protected_data+=add_on;
        cout << protected_data << endl;
    }
private:
    string private_data = "private data";
protected:
    string protected_data = "protected data";
public:
    string public_data = "public data";
};

class N : public M {
};

int main(){
    M m;
    cout << "public data = " << m.public_data << endl;

    N n;
    n.print_protected();
    n.print_modified_protected("->n_instance");

    N o;
    o.print_protected();
    o.print_modified_protected("->o_instance");
}
