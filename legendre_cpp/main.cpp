#include <cmath>
#include <iostream>

#include <boost/math/special_functions/legendre.hpp>


double P3(double x)
{
    return 0.5 * (5 * std::pow(x, 3) - 3 * x);
}
 
double P4(double x)
{
    return 0.125 * (35 * std::pow(x, 4) - 30 * x * x + 3);
}
 
int main()
{
    // spot-checks
    std::cout << boost::math::legendre_p(3, 0.25) << '=' << P3(0.25) << '\n'
              << boost::math::legendre_p(4, 0.25) << '=' << P4(0.25) << '\n';
}