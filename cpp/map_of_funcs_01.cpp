#include <iostream>
#include <functional>
#include <vector>
#include <array>
#include <map>

namespace esempio{
    void func(std::vector< std::array<int,3> > &coord_result_energy,
              std::array<int,3> &arr){

        coord_result_energy.push_back({0,1,2});
        coord_result_energy.push_back({3,4,5});
        coord_result_energy.push_back(arr);
    }
}



typedef  std::map<std::string,std::function<void(std::vector< std::array<int,3> > &coord_result_energy,
                                                 std::array<int,3> &arr)>> str_to_func;


str_to_func map_funcs_to_directions_energy_coords() {

    str_to_func stringa_to_funzione;

    stringa_to_funzione["north-west"] = [](std::vector< std::array<int,3> > &coord_result_energy,
                                           std::array<int,3> &arr) {

        coord_result_energy.push_back({0,1,2});
        coord_result_energy.push_back({3,4,5});
        coord_result_energy.push_back(arr);
    };

    stringa_to_funzione["north-east"] = esempio::func;

    return stringa_to_funzione;
}

int main() {

    auto mappa =  map_funcs_to_directions_energy_coords();

    std::vector<std::array<int,3>> a;

    std::cout << "size a = " << a.size() << std::endl;

    std::array<int,3> b = {33,44,55};

    mappa["north-west"](a,b);

    std::cout << "size a = " << a.size() << std::endl;

    mappa["north-east"](a,b);

    std::cout << "size a = " << a.size() << std::endl;

    //mappa[]
    return 0;
}
