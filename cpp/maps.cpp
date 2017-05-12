#include <iostream>
#include <map>
#include <vector>
#include <array>

using namespace std;

//map<int, char> m = {{1, 'a'}, {3, 'b'}, {5, 'c'}, {7, 'd'}};

/*
    return 0;
}*/


namespace test{
    map<string,vector<double>> mappa_vec = {{"here",{0,2}},{"we",{1,5}},{"go",{2,9}}};
    map<string,array<double,2>> mappa_arr = {{"here",{0,2}},{"we",{1,5}},{"go",{2,9}}};
}

void print_map(){
    cout << "this is a good spot for a template exercise" << endl;
}

int main(){

    cout << "stocazzo" << endl;

    auto it = ::test::mappa_vec.begin();
    for (; it!=::test::mappa_vec.end(); ++it){
        cout << "map = "<< it->first  << ", "
             << it->second[0] << ", "<< it->second[1] << endl;
    }

    auto it_ = ::test::mappa_arr.begin();
    for (; it_!=::test::mappa_arr.end(); ++it_){
        cout << "map = "<< it_->first  << ", "
             << it_->second[0] << ", "<< it_->second[1] << endl;
    }

    return 0;
}
