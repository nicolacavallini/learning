#include <iostream>
#include <vector>
#include <functional>
#include <math.h>

using namespace std;

template<class T>
vector<T> linspace(const T y_min = 0,
                   const T y_max = 1,
                   const int n_samples=10) {

    vector<T> y(n_samples);

    const T delta = (y_max - y_min)/(T)(n_samples-1);

    for (int i =0 ; i < n_samples; i++) {
        T v = y_min + delta * (T)i;
        y[i] = v;
    }

    return y;
}


vector<double> construct_filter(
        const function<double(const double)> &func,
        const int n_intervals,
        const double delta){

    vector<double> out;
    for (int i = 0; i < n_intervals; i++){
        double t = delta/2. + (double)i*delta;
        out.push_back(func(t));
    }

    return out;
}

vector<double> forward_convolution(
    const vector<double> & signal,
    const vector<double> &filter) {

    vector<double> convolution(signal.size());

    int n = filter.size();
    int s = signal.size();

    for (int i = 0; i < signal.size(); i++) {
        double c = 0;
        int I = floor((double)(i+n)/(double)s);
        int end = (i+n) - I*(i+n-s);

        //cout << "n = " << n ;
        //cout << ", i = " << i;
        //cout << ", i+n = " << i+n ;
        //cout << ", I = " << I;
        //cout << ", i+n-s = " << i+n-s ;
        //cout << ", end = " <<
        //cout << endl;

        //int end = (i+filter.size())%signal.size();
        //cout << ", t  = ";
        for (int t = i;
             t < end;
             t++) {
            c+= signal[t] * filter[i % filter.size()];
            //cout << t << ", ";
        }
        //cout << endl;
        convolution[i] = c;
    }
    return convolution;
}

vector<double> backward_convolution(
    const vector<double> & signal,
    const vector<double> &filter) {

    vector<double> convolution(signal.size());

    int n = filter.size();
    int s = signal.size();

    for (int i = s-1; i >=0; i--) {
        double c = 0;
        //int I = floor((double)(i+n)/(double)s);
        //int end = (i+n) - I*(i+n-s);

        //cout << "n = " << n ;
        //cout << ", i = " << i;
        //cout << ", i-n = " << i-n ;
        int end = (i-n) > 0 ? (i-n) : 0;
        //cout << ", end = " << end ;
        //cout << ", I = " << I;
        //cout << ", i+n-s = " << i+n-s ;
        //cout << ", end = " <<
        //cout << endl;

        //int end = (i+filter.size())%signal.size();
        //cout << " -> ";
        int f = n-1;
        for (int t = i;
             t > end;
             t--) {

            c+= signal[t] * filter[f];
            //cout << "t = " << t;
            //cout << ", f = " <<  f << ", ";
            f--;
        }
        //cout << endl;
        convolution[i] = c;
    }
    return convolution;
}

template<class T>
void print_vec(const vector<T> &vec)
{
    cout << "size = " << vec.size() <<endl;
    cout << "["<< endl;
    for (const T &entry : vec)
        cout << entry << endl;
    cout << "]"<< endl;
}



int main() {

    vector<double> vec = {0,0,0,1,3,0,0,0,0,0};

    print_vec(vec);

    function<double(const double)> f = [](const double a ){
        return 1.;
    };

    vector<double> filter = construct_filter(f,4,1);

    print_vec(filter);

    vector<double> f_conv = forward_convolution(vec,filter);

    print_vec(f_conv);

    vector<double> b_conv = backward_convolution(vec,filter);

    print_vec(b_conv);



    return 0;
}
