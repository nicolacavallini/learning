#include <iostream>
//#include <stdlib.h>
#include <thread>
//#include <pthread.h>
#include <vector>

/*template<class InputIt, class Function>
inline
Function
parallel_fun(InputIt first, InputIt last, Function f)
{
    std::thread threads[4];
    for (int i=0; first != last; ++first)
    {
        threads[i] = std::thread(f);
        i++;
    }
    for (int i=0; i<4;i++)
    {
        threads[i].join();
    }
    return f;
}


int main()
{
    int z=90;
    std::vector<int> a(4);
    a[0]=1; a[1]=2;
    parallel_fun(a.begin(), a.end(), [=]()
                                      {
                                        std::cout<<"test" << z << std::endl;
                                        //do something
                                      });
}*/


int main(){
    std::cout << "stocazzo" << std::endl;

    const int num_threads = 2;

    long long unsigned int sum[num_threads] = {0,0};
    unsigned int N = 1e8;

    auto add_fist_half = [&](){
        for (unsigned int i =0; i<N/2; i++)
            sum[0] += i*i;
    };

    auto add_second_half = [&](){
        for (unsigned int i =N/2; i<N; i++)
            sum[1] += i*i;
    };

    std::thread threads[num_threads];

    threads[0] = std::thread(add_fist_half);

    threads[1] = std::thread(add_second_half);

    for (int i=0; i<num_threads;i++)
        threads[i].join();

    std::cout << "sum = " << sum[0]+sum[1] << std::endl;

    return 0;
}
