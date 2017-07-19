#include <iostream>
#include <pthread.h>

long long unsigned int sum[2] = {0,0};
unsigned int N = 1e8;

void * add_first_half(void *){

    for (unsigned int i =0; i<N/2; i++)
        sum[0] += i*i;

    return 0;
}

void * add_second_half(void *){

    for (unsigned int i =N/2; i<N; i++)
        sum[1] += i*i;


    return 0;
}


int main(){
    pthread_t t1, t2;

    pthread_create(&t1, 0, &add_first_half, 0);
    pthread_create(&t2, 0, &add_second_half, 0);

    pthread_join(t1,0);
    pthread_join(t2,0);

    std::cout << "sum = " << sum[0]+sum[1] << std::endl;
    return 0;
}
