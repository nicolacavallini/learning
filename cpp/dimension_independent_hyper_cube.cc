#include <iostream>
#include <array>
#include <vector>
#include <string>

template<int dim>
void print_array(const std::array<int,dim> &a,
                 const std::string name="a" ){
    for (int d = 0; d < dim; d++){
        std::cout << name << "[" << d << "] = "<< a[d] << std::endl;
    }
}

template<class T>
void print_vector(const std::vector<T> &v,
                 const std::string name="v" ){
    std::cout << "---" << std::endl;
    for (int d = 0; d < v.size(); d++){
        std::cout << name << "[" << d << "] = "<< v[d] << std::endl;
    }
    std::cout << "---" << std::endl;
}


std::array<int,3> direction_indices(const int dir){

    std::array<int,3> indeces;
    int i = 0;
    for (int d = 2; d >=0 ; d--){
        if (d!=dir){
            indeces[i] = d;
            i++;
        }
    }
    indeces[2] = dir;

    return indeces;
}

/**
 * dims = [a,b,c] -> delta = [1,a,a*b];
 */
template<int dim>
std::array<int,dim> get_direction_deltas(const std::array<int,dim> &dims){
    std::array<int,dim> delta;
    delta.fill(1);
    for (int d = 1; d < dim ; d++)
        delta[d] = delta[d-1]*dims[d-1];
    return delta;
}

void test_direction_index(){

}
template<int dim>
void hyper_rectangle(const std::array<int,dim> &dimensions){

    std::array<int,3> dims;
    dims.fill(1);
    for (int d = 0; d < dim; d++)
        dims[d] = dimensions[d];

    const int coord = 0;

    int length = 1;
    for (int d = 0; d < dim; d++)
        length*=dims[d];
    std::array<std::vector<double>,dim> cp;
    for (int coord = 0; coord<dim; coord++)
        cp[coord].reserve(length);

    for (int coord = 0; coord<dim; coord++){
        std::array<int,3> idx = direction_indices(coord);
        for (int i = 0; i < dims[idx[0]]; i++)
            for (int j = 0; j < dims[idx[1]]; j ++)
                for (int k = 0; k < dims[idx[2]]; k++)
                    cp[coord].push_back((double)k);
    }

    std::array<std::string,3> coord_names = {"x","y","z"};
    for (int d = 0; d < dim; d++)
        print_vector<double>(cp[d],coord_names[d]);
}

void explore_space(){


    const int dim = 3;
    std::array<int,dim> dims = {5,4,3};

    print_array<3>(dims,"dims");


    std::array<int,dim> delta = get_direction_deltas<3>(dims);
    print_array<dim>(delta,"delta");
    std::array<int,3> idx = direction_indices(2);

    for (int i = 0; i < dims[idx[0]]; i++){
        for (int j = 0; j < dims[idx[1]]; j ++){

            int s = i * delta[idx[0]] + j * delta[idx[1]] ;

            std::vector<int> curve_ids(0);

            for (int k = 0; k < dims[idx[2]]; k++){
                curve_ids.push_back(s);
                s+=delta[idx[2]];
            }
            print_vector(curve_ids,"curve_ids");
        }
    }
}

int main(){

//    const int dim = 3;
//    std::array<int,dim> dims = {5,4,3};

//    print_array<3>(dims,"dims");

//    for (int d = 0; d < dim ; d++){
//        std::array<int,3> idx = direction_indices(d);
//        std::string name = "idx-" + std::to_string(d);
//        print_array<3>(idx,name);
//    }

    std::array<int,3> dims03 = {5,4,3};
    hyper_rectangle<3>(dims03);

    std::array<int,2> dims02 = {5,4};
    hyper_rectangle<2>(dims02);

    //explore_space();

//    std::array<int,dim> delta = get_direction_deltas<3>(dims);
//    print_array<dim>(delta,"delta");
//    std::array<int,3> idx = direction_indices(2);

//    for (int i = 0; i < dims[idx[0]]; i++){
//        for (int j = 0; j < dims[idx[1]]; j ++){

//            int s = i * delta[idx[0]] + j * delta[idx[1]] ;

//            std::vector<int> curve_ids(0);

//            for (int k = 0; k < dims[idx[2]]; k++){
//                curve_ids.push_back(s);
//                s+=delta[idx[2]];
//            }
//            print_vector(curve_ids,"curve_ids");
//        }
//    }

    //

    return 0;
}
