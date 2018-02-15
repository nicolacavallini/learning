#include <vector>
#include <array>
#include <iostream>
#include <map>

using namespace std;

double double_rand(double min=0., double max=1.)
{
    double f = (double)rand() / RAND_MAX;
    return min + f * (max - min);
}

void matte_style(const long long unsigned int N,
                 const vector<array<double,3>> &pcl_matte){

    double sum = 0;
    for (const auto p : pcl_matte)
        sum += p[0];
}


void nico_style(const long long unsigned int N,
                map<string,vector<double>> &pcl_nico){

    double sum = 0;
    for (const auto x : pcl_nico["x"])
        sum += x;
}



int main(){

    long long unsigned int N =1e+7;

    vector<array<double,3>> pcl_matte;

    for (int i = 0; i < N; i++)
        pcl_matte.push_back({double_rand(),double_rand(),double_rand()});

    matte_style(N,pcl_matte);

    cout << "matte style done" << endl;


    map<string,vector<double>> pcl_nico;

    for (int i = 0; i < N; i++){
        pcl_nico["x"].push_back(double_rand());
        pcl_nico["y"].push_back(double_rand());
        pcl_nico["z"].push_back(double_rand());
    }


    nico_style(N,pcl_nico);

    cout << "nico style done" << endl;





    return 0;
}
