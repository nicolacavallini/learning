#include <fstream>
#include <iostream>

#include <vector>

template<class T>
void write_vector_to_binary(const std::string filenmame,
                      const std:: vector<T> &vec) {
    std::ofstream file(filenmame, std::ios::binary);

    for (const double& v : vec) file.write(reinterpret_cast<const char*>( &v )
        ,sizeof(T));
}

template<class T>
std::vector<T> read_binary_to_vector(const std::string filenmame){
    std::vector<T> vec;

    std::ifstream file( filenmame, std::ios::binary );
    T v;

    while( file.read( reinterpret_cast<char*>( &v ), sizeof(T) ) )
       vec.push_back(v) ;

   return vec;
}

int main()
{
    std::cout << "stocazzo" << std::endl;
    std::vector<double> vec;
    vec.push_back(1.0);
    vec.push_back(2.0);
    vec.push_back(3.0);

    write_vector_to_binary("outfile.dat",vec);

    std::vector<double> read =
    read_binary_to_vector<double>("outfile.dat");

    for (auto const& r: read)
        std::cout << "value = " << r << std::endl;

    return 0;
}
