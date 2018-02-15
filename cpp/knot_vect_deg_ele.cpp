#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <algorithm>


template<class T>
void print_vector(const std::vector<T> &v,
                 const std::string name="v" ){
    std::cout << "---" << std::endl;
    for (int d = 0; d < v.size(); d++){
        std::cout << name << "[" << d << "] = "<< v[d] << std::endl;
    }
    std::cout << "---" << std::endl;
}


int main(){

    std::vector<double> v {0,0,0,.5,1,1,1};
    int deg_elev = 2;
    print_vector(v);

    double k0 = v[0];

    int n = v.size();

    for (int i = 0 ; i  < n; i++){
        if (v[i] != k0)
            for (int j = 0; j < deg_elev; j++)
                v.push_back(k0);

        k0=v[i];
    }
    for (int j = 0; j < deg_elev; j++)
        v.push_back(k0);
    std::sort(v.begin(),v.end());

    print_vector(v);


    return 0;
}
