#include <iostream>

template<typename T>
struct point_t
{
    T x, y, z;
};

struct point_int
{
    int x, y, z;
};

int main()
{

    std::cout << "is the memory foot print of the template class the same as the non templete one? " << std::boolalpha << (sizeof(point_int) == sizeof(point_t<int>)) << std::endl;

    point_t<int>  pt = {10, 20, 30};

    std::cout << "is the memory consuption of the instantiated classes the same? " << std::boolalpha << (sizeof(point_int) == sizeof(point_t<int>)) << std::endl;

    std::cout << "if that is true i can cast the template class in the non template one:" << std::endl;

    point_int  pi = *reinterpret_cast<point_int*>(&pt);

    std::cout << "{" << pi.x <<"," << pi.y <<"," << pi.z << "}" << std::endl;

    std::cout << "cool that's it!" << std::endl;

}
