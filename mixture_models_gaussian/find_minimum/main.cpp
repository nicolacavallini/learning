#include <iostream>

#include <math.h>

#include <algorithm>
#include <functional>
#include <map>

#include <vector>
#include <array>

#include <assert.h>

template <typename T>
std::vector<size_t> sort_indices(std::vector<T> &v) {

    // initialize original index locations
    std::vector<size_t> idx(v.size());
    std::iota(idx.begin(), idx.end(), 0);

    // sort indexes based on comparing values in v
    std::sort(idx.begin(), idx.end(),
    [&v](size_t i1, size_t i2) {
        return v[i1] < v[i2];
    });

    std::sort(v.begin(), v.end());

    return idx;
}

class BimodalGaussian{
public:
    BimodalGaussian(const std::vector<double> & means,
                    const std::vector<double> & variances);
    void sort_stationary_points();
private:
    std::vector<double> values;
    std::vector<std::string> ids;
    std::string stationary_points_description;

    std::map<std::string,double> initial_condition_selector;

    void initialize_ic_selector(std::vector<double> m,
                                std::vector<double> v,
                                std::vector<double> s);

};

BimodalGaussian::BimodalGaussian(const std::vector<double> & means, const std::vector<double> & variances){

    assert(means.size()==variances.size());
    assert(means.size()==2);
    assert(variances.size()==2);

    std::vector<double> m = means;

    const auto id = sort_indices(m);

    std::vector<double> s = {
        std::sqrt(variances[id[0]]),std::sqrt(variances[id[1]])};

    std::vector<double> v = {
        variances[id[0]],variances[id[1]]};

    values = {m[0],m[0]+s[0],m[1]-s[1],m[1]};
    ids = {"m0","s0","s1","m1"};

    initialize_ic_selector(m,v,s);
}

void BimodalGaussian::sort_stationary_points(){

    const auto id = sort_indices(values);

    for (const auto i : id)
        stationary_points_description += ids[i];

    std::cout << stationary_points_description << std::endl;
}

void BimodalGaussian::initialize_ic_selector(
        std::vector<double> m,
        std::vector<double> v,
        std::vector<double> s){

    initial_condition_selector["m0s0s1m1"] =
            m[0]+s[0]+v[0];

}

std::function<double(const double)> get_gaussian_function(
    const double m/*mean*/,
    const double v/*variance*/,
    std::string derivative="function") {
    std::map<std::string,std::function<double(const double)>> func_list;
    func_list["function"] = [m,v](const double z) {
        return
            0.707106781186547*exp(
                -1.0/2.0*pow(-m + z, 2)/v)/(sqrt(M_PI)*sqrt(v));
    };

    func_list["prime"] = [m,v](const double z) {
        return
            -0.353553390593274*(-2*m + 2*z)*exp(-1.0/2.0
                                                *pow(-m + z, 2)/v)/(sqrt(M_PI)*pow(v, 3.0/2.0));
    };

    func_list["prime_prime"] = [m,v](const double z) {
        return
            0.707106781186547*(-1 + pow(m - z, 2)/v)
            *exp(-1.0/2.0*pow(m - z, 2)/v)/(sqrt(M_PI)*pow(v, 3.0/2.0));
    };

    return func_list[derivative];
}

std::function<double(const double)> get_gaussian_mixture(
        std::vector<double> & means,
        std::vector<double> & variances,
        std::string derivative="function") {

    std::vector<std::function<double(const double)>> gm_vec;

    for (int i = 0; i<means.size() ; i++ )
        gm_vec.push_back(get_gaussian_function(means[i],variances[i],derivative));

    std::function<double(const double)> out = [gm_vec](const double z) {
        double out = 0;
        for (const auto & f : gm_vec)
            out+=f(z);
        return out;
    };

    return out;
}

std::function<double(const double)> get_two_gaussians_difference(
        std::vector<double> & means,
        std::vector<double> & variances,
        std::string derivative="function") {

    assert(means.size()==variances.size());
    assert(means.size()==2);
    assert(variances.size()==2);

    std::vector<double> m = means;

    const auto id = sort_indices(m);

    std::vector<double> v = {
        variances[id[0]],variances[id[1]]};


    std::vector<std::function<double(const double)>> gm_vec;

    for (int i = 0; i<means.size() ; i++ )
        gm_vec.push_back(get_gaussian_function(m[i],v[i],derivative));

    std::function<double(const double)> out = [gm_vec](const double z) {
        return gm_vec[1](z)-gm_vec[0](z);
    };

    return out;
}


void test_gaussian() {
    std::cout << "gaussian" << std::endl;
    double m0 = 3;
    double s0 = .25;
    std::function<double(const double)> f = get_gaussian_function(m0,s0);
    std::function<double(const double)> df = get_gaussian_function(m0,s0,"prime");
    std::function<double(const double)> ddf = get_gaussian_function(m0,s0,"prime_prime");
    std::cout << "f0(3.1) = " << f(3.1) << std::endl;
    std::cout << "f0(3.1) = " << df(3.1) << std::endl;
    std::cout << "f0(3.1) = " << ddf(3.1) << std::endl;
}

void test_gaussian_mixture() {
    std::vector<double> means = {3,6};
    std::vector<double> variances = {.25,1};

    const auto gm = get_gaussian_mixture(means,variances);
    std::cout << "   gm(5) = " << gm(5) << std::endl;

    const auto d_gm = get_gaussian_mixture(means,variances,"prime");
    std::cout << " d_gm(5) = " << d_gm(5) << std::endl;

    const auto dd_gm = get_gaussian_mixture(means,variances,"prime_prime");
    std::cout << "dd_gm(5) = " << dd_gm(5) << std::endl;
}

double two_gaussians_intersection(std::vector<double> & means,
                                  std::vector<double> & variances){

    std::function<double(const double)> tgd =
            get_two_gaussians_difference(means,variances);

    std::function<double(const double)> d_tgd =
            get_two_gaussians_difference(means,variances,"prime");

    std::cout << "two_gaussians_difference  = " << tgd(5) << std::endl;
    std::cout << "d_two_gaussians_difference  = " << d_tgd(5) << std::endl;

    double z0 = .5*(means[0]+means[1]);
    double z1;

    for (int i = 0; i < 10; i++){
        z1 = z0 - tgd(z0)/d_tgd(z0);
        z0 = z1;
        if (std::abs(tgd(z0)) < 1e-10)
            break;
    }

    return z0;
}



int main() {

    test_gaussian();

    test_gaussian_mixture();

    std::vector<double> means = {6,3};
    std::vector<double> variances = {1,.25};

    double intersection = two_gaussians_intersection(
                means,variances);

    return 0;
}
