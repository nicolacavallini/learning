#include <iostream>
#include <vector>

using namespace std;

int main(){
    vector<double> v;
    v.reserve(500);

    cout << "v.size() = " << v.size() << endl;
    cout << "v.capacity() = " << v.capacity() << endl;

    v.push_back(5);
    v.shrink_to_fit();

    cout << "v.capacity() = " << v.capacity() << endl;

    vector<vector<int> > vv(3,vector<int>(5));

    cout << "vv.capacity() = " << vv.capacity() << endl;

    vector<double> w = {1,2,3}; 
    cout << "w.size() = " << w.size() << endl;
    w.clear();
    cout << "w.size() = " << w.size() << endl;
    w.resize(3);
    cout << "w.size() = " << w.size() << endl;
    //w.erase();
    //cout << "w.size() = " << w.size() << endl;

    return 0;
}
