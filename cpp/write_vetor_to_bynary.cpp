#include <fstream>
#include <iostream>

#include <vector>

int main()
{
    std::cout << "stocazzo" << std::endl;
    std::vector<double> vec;
    vec.push_back(1.0);
    vec.push_back(2.0);
    vec.push_back(3.0);

    std::ofstream file("outfile.dat", std::ios::binary);

    for (const double& v : vec) file.write(reinterpret_cast<const char*>( &v )
        ,sizeof(double));

    return 0;
}
