#include <iostream>

#include <boost/math/special_functions/binomial.hpp>

int main(){

    double binomial = boost::math::binomial_coefficient<double>(10, 2);

    std::cout << "binomial(10,2) = " << binomial << std::endl;
    return 0;
}
