#include <iostream>
#include <map>
#include <vector>

using namespace std;

//map<int, char> m = {{1, 'a'}, {3, 'b'}, {5, 'c'}, {7, 'd'}};

/*
    return 0;
}*/


namespace test{
    map<string,vector<double>> mappa = {{"here",{0}},{"we",{1}},{"go",{2}}};
}

int main(){

    auto it = ::test::mappa.begin();
    for (; it!=::test::mappa.begin(); ++it){
        cout << "map = "<< it->first  << endl; //<< ", "<< it->second <<endl;
    }
    return 0;
}
