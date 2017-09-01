#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <random>

#include <vector>
#include <iostream>
#include <iomanip>

typedef float data_t;

using namespace std;

template<class T>
void print_vec(const vector<T> &vec)
{
    cout << "size = " << vec.size() <<endl;
    cout << "["<< endl;
    for (const T &entry : vec)
        cout << entry << endl;
    cout << "]"<< endl;
}

template<class T>
void print_vec_vec(vector<vector<T> > &vec)
{
    for (const auto &row : vec){
        for (const auto &el : row){
            cout << el << ", ";
        }
        cout << "stocazzo"<<  endl;
    }
}

template<class T>
vector<vector<T> > init_vec_vec(const int rows,
                                const int cols)
{
    vector<vector<T> > vec(rows,vector<T>(cols));

    for (int i = 0; i <rows; i++){
        for (int j =0; j < cols; j++){
            vec[i][j] = 0;
        }

    }
    return vec;
}

data_t get_gaussian_random(const data_t mean, const data_t std) {
    std::random_device rd;
    std::mt19937 e2(rd());
    std::normal_distribution<data_t> gen(mean, std);

    return gen(e2);
}

vector<data_t> generate_1d_data(const data_t mean,
                                const data_t std,
                                const int count) {
    vector<data_t> data(count);
    for(int i=0;i<data.size();i++) {
        data_t noise = 2*std * (data_t)rand()/RAND_MAX;
        data[i] = get_gaussian_random(mean, std)+noise;
    }

    return data;
}

vector<data_t>get_data_means_var(
        const int num_components) {

    vector<data_t> ret(2*num_components);

    for(int i=0;i<2*num_components;i+=2) {
        ret[i] = 640*i/(2.0*num_components) + 120;
        ret[i+1] = 1000;
    }

    return ret;
}

data_t calculate_1d_probability(
        const data_t value,
        const data_t mean,
        const data_t std) {
    data_t ret = 0.0;

    data_t frac = 1.0 / (std * sqrt(2 * M_PI));
    data_t power = -0.5 * pow((value - mean) / std, 2.0);

    ret = frac * exp(power);

    return ret;
}

void draw_1d_data_mixture(cv::Mat img,
                          data_t* data, data_t* weights,
                          int samples_per_component, int num_components){
    std::vector<cv::Scalar> palette;
    palette.push_back(cv::Scalar(0, 0, 255));   // red
    palette.push_back(cv::Scalar(255, 0, 0));
    palette.push_back(cv::Scalar(0, 255, 0));
    palette.push_back(cv::Scalar(0, 0, 0));
    palette.push_back(cv::Scalar(192, 192, 192));

    int num_samples = samples_per_component * num_components;
    for(int i=0;i<num_samples;i++) {
        data_t* wt = &weights[i*num_components];

        cv::Scalar color = cv::Scalar(0);
        for(int j=0;j<num_components;j++) {
            color += wt[j] * palette[j];
        }

        data_t pt = data[i];
        cv::line(img, cv::Point(pt, 10), cv::Point(pt, 20), color);
    }
}

void draw_1d_gaussian_mixture(cv::Mat img, data_t* mean, data_t* stddev, data_t* pi, int num_components) {
    data_t* prob = new data_t[img.cols];
    data_t* frac = new data_t[num_components];

    for(int j=0;j<num_components;j++) {
        frac[j] = 1.0 / (stddev[j] * sqrt(2*M_PI));
    }

    data_t min_prob = FLT_MAX;
    data_t max_prob = FLT_MIN;
    memset(prob, 0, sizeof(data_t)*img.cols);

    for(int x=0;x<img.cols;x++) {

        data_t tempsum = 0.0;

        for(int j=0;j<num_components;j++)
        {
            tempsum = tempsum + pi[j] * frac[j] * exp(-0.5 * (x-mean[j])*(x-mean[j])/(stddev[j]*stddev[j]));
        }
        prob[x] = tempsum;

        if(prob[x] > max_prob)
            max_prob = prob[x];

        if(prob[x] < min_prob)
            min_prob = prob[x];
    }
    data_t prev = 0;
    for(int x=0;x<img.cols;x++) {
        data_t p = 30 + 100*(1-(prob[x]/(max_prob-min_prob)));
        cv::line(img, cv::Point(x-1, prev), cv::Point(x, p), cv::Scalar(0));
        prev = p;
    }

    for(int i=0;i<num_components;i++) {
        cv::line(img, cv::Point(mean[i], 30), cv::Point(mean[i], 130), cv::Scalar(0));
        cv::line(img, cv::Point(mean[i]-stddev[i], 30), cv::Point(mean[i]-stddev[i], 130), cv::Scalar(128));
        cv::line(img, cv::Point(mean[i]+stddev[i], 30), cv::Point(mean[i]+stddev[i], 130), cv::Scalar(128));
    }
}

void run_gaussian_mixture_1d() {
    //const int num_components = 5;


    //vector<data_t> data(num_components*samples_per_component);

    vector<data_t> mean = {120, 480};//{120, 480, 240, 360, 320};//
    vector<data_t> std = {20, 30};//{20, 30, 10, 40, 5};//
    int num_components = mean.size();


    const int samples_per_component = 60;
    vector<data_t> data;

    for(int i=0; i<mean.size();i++){
        for (int j=0 ; j< samples_per_component ; j++){
            data.push_back(get_gaussian_random(mean[i],std[i]));
        }
    }
        //data[i] = generate_1d_data(mean[i], std[i], samples_per_component);


    //print_vec(data);


    vector<data_t> initial_guess = get_data_means_var(
                num_components);

    //print_vec(initial_guess);


    int num_samples = num_components * samples_per_component;
    //data_t* responsibility = new data_t[num_components*num_samples];

    vector<vector<data_t>> responsibility =
            init_vec_vec<data_t>(num_components,num_samples);
            //responsibility(num_components,
            //vector<data_t>(num_samples));
    vector<data_t> pi (num_components);
    vector<data_t> learned_means (num_components);
    vector<data_t> learned_std(num_components);

    for(int i=0;i<num_components;i++) {
        learned_means[i] = initial_guess[2*i];
        learned_std[i] = sqrt(initial_guess[2*i+1]);
        pi[i] = 1.0/num_components;
    }



    for(int iter=0;iter<101;iter++) {
        // E step
        for(int i=0;i<num_samples;i++) {
            data_t total_prob = 0.0f;
            for(int j=0;j<num_components;j++) {
                data_t prob = pi[j] *
                        calculate_1d_probability(
                            data[i], learned_means[j], learned_std[j]);
                total_prob += prob;
                responsibility[j][i] = prob;
            }

            if(total_prob > 0) {
                for(int j=0;j<num_components;j++) {
                    responsibility[j][i] /= total_prob;
                }
            }
        }

        vector<data_t> nc(num_components);

        for(int i=0;i<num_samples;i++) {
            for(int j=0;j<num_components;j++) {
                nc[j] += responsibility[j][i];
            }
        }

        for(int j=0;j<num_components;j++) {
            pi[j] = nc[j]/(data_t)num_samples;
        }

        for(int j=0;j<num_components;j++) {
            learned_means[j] = 0;
            for(int i=0;i<num_samples;i++) {
                learned_means[j] += responsibility[j][i] * data[i];
            }

            if(nc[j] > 0) {
                learned_means[j] /= nc[j];
            }

            for(int i=0;i<num_samples;i++) {
                data_t diff = data[i] - learned_means[j];
                learned_std[j] += responsibility[j][i] * diff * diff;
            }

            if(nc[j] > 0) {
                learned_std[j] /= nc[j];
            }

            learned_std[j] = sqrt(learned_std[j]);
        }

        /*const int graph_width = 640;
        const int graph_height = 140;
        cv::Mat graph(graph_height, graph_width, CV_8UC3, cv::Scalar(255, 255, 255));
        draw_1d_data_mixture(graph, data, responsibility, samples_per_component, num_components);
        draw_1d_gaussian_mixture(graph, learned_means, learned_std, pi, num_components);

        char file[100];
        sprintf(file, "./mixture-gaussian-iter%02d.png", iter);
        cv::imwrite(file, graph);*/

        cout <<"Iteration = " << iter << endl;


        for(int j=0;j<num_components;j++)

            cout << "Learned " << j <<
                    " - mean = " <<
                    learned_means[j] <<
                    ", std = " << learned_std[j] <<
                    " , pi = " << pi[j] << endl;

    }

}

int main(int argc, char* argv[]) {
    run_gaussian_mixture_1d();
    return 0;
}
