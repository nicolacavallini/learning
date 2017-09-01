#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <random>

//#include <vector>
//#include <iostream>
//#include <iomanip>

typedef double data_t;

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

float get_gaussian_random(float mean, float std) {
    std::random_device rd;
    std::mt19937 e2(rd());
    std::normal_distribution<float> gen(mean, std);

    return gen(e2);
}

float* generate_1d_data(float mean, float std, int count) {
    float* data = new float[count];
    for(int i=0;i<count;i++) {
        float noise = 2*std * (float)rand()/RAND_MAX;
        //noise -= 10;
        data[i] = get_gaussian_random(mean, std)+noise;
    }

    return data;
}

float* get_data_means_var(float* data, int count_per_component, int num_components) {
    float* ret = new float[2*num_components];

    for(int i=0;i<2*num_components;i+=2) {
        ret[i] = 640*i/(2.0f*num_components) + 120;
        ret[i+1] = 1000;
    }

    return ret;
}

float calculate_1d_probability(float value, float mean, float std) {
    float ret = 0.0f;

    float frac = 1.0f / (std * sqrt(2 * M_PI));
    float power = -0.5f * pow((value - mean) / std, 2.0);

    ret = frac * exp(power);

    return ret;
}

void draw_1d_data_mixture(cv::Mat img,
                          float* data, float* weights,
                          int count_per_component, int num_components){
    std::vector<cv::Scalar> palette;
    palette.push_back(cv::Scalar(0, 0, 255));   // red
    palette.push_back(cv::Scalar(255, 0, 0));
    palette.push_back(cv::Scalar(0, 255, 0));
    palette.push_back(cv::Scalar(0, 0, 0));
    palette.push_back(cv::Scalar(192, 192, 192));

    int total = count_per_component * num_components;
    for(int i=0;i<total;i++) {
        float* wt = &weights[i*num_components];

        cv::Scalar color = cv::Scalar(0);
        for(int j=0;j<num_components;j++) {
            color += wt[j] * palette[j];
        }

        float pt = data[i];
        cv::line(img, cv::Point(pt, 10), cv::Point(pt, 20), color);
    }
}

void draw_1d_gaussian_mixture(cv::Mat img, float* mean, float* stddev, float* pi, int num_components) {
    float* prob = new float[img.cols];
    float* frac = new float[num_components];

    for(int j=0;j<num_components;j++) {
        frac[j] = 1.0 / (stddev[j] * sqrt(2*M_PI));
    }

    float min_prob = FLT_MAX;
    float max_prob = FLT_MIN;
    memset(prob, 0, sizeof(float)*img.cols);

    for(int x=0;x<img.cols;x++) {

        float tempsum = 0.0;

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
    float prev = 0;
    for(int x=0;x<img.cols;x++) {
        float p = 30 + 100*(1-(prob[x]/(max_prob-min_prob)));
        cv::line(img, cv::Point(x-1, prev), cv::Point(x, p), cv::Scalar(0));
        prev = p;
    }

    for(int i=0;i<num_components;i++) {
        cv::line(img, cv::Point(mean[i], 30), cv::Point(mean[i], 130), cv::Scalar(0));
        cv::line(img, cv::Point(mean[i]-stddev[i], 30), cv::Point(mean[i]-stddev[i], 130), cv::Scalar(128));
        cv::line(img, cv::Point(mean[i]+stddev[i], 30), cv::Point(mean[i]+stddev[i], 130), cv::Scalar(128));
    }
}

void print_vec(float* v, int size){

    for (int i = 0; i< size; i++){
        printf("v = %f\n",v[i]);
    }
}



void run_gaussian_mixture_1d() {
    const int num_components =5;
    const int count_per_component = 100;
    float *data = new float[num_components*count_per_component];

    float mean[5] = {120, 480, 240, 360, 320};
    float std[5] = {20, 30, 10, 40, 5};

    for(int i=0;i<num_components;i++) {
        float* new_data = generate_1d_data(mean[i], std[i], count_per_component);
        memcpy(data+(count_per_component*i), new_data, sizeof(float)*count_per_component);
    }

    float* initial_guess = get_data_means_var(data, count_per_component, num_components);




    int total = num_components * count_per_component;
    float* responsibility = new float[num_components*total];
    float* pi             = new float[num_components];
    float* learned_means  = new float[num_components];
    float* learned_std    = new float[num_components];

    for(int i=0;i<num_components;i++) {
        learned_means[i] = initial_guess[2*i];
        learned_std[i] = sqrt(initial_guess[2*i+1]);
        pi[i] = 1.0f/num_components;
    }

    for(int iter=0;iter<1;iter++) {
        // E step
        for(int i=0;i<total;i++) {
            float total_prob = 0.0f;
            for(int j=0;j<num_components;j++) {
                float prob = pi[j] *
                        calculate_1d_probability(
                            data[i], learned_means[j], learned_std[j]);
                total_prob += prob;
                responsibility[i*num_components+j] = prob;
            }

            if(total_prob > 0) {
                for(int j=0;j<num_components;j++) {
                    responsibility[i*num_components + j] /= total_prob;
                }
            }
        }

        //if (iter==100)
        //    print_vec(responsibility,num_components*total);






        // M step
        float* nc = new float[num_components];
        memset(nc, 0, sizeof(float) * num_components);
        for(int i=0;i<total;i++) {
            for(int j=0;j<num_components;j++) {
                nc[j] += responsibility[i*num_components+j];
            }
        }

        print_vec(nc,num_components);

        for(int j=0;j<num_components;j++) {
            pi[j] = nc[j]/total;
        }

        for(int j=0;j<num_components;j++) {
            learned_means[j] = 0;
            for(int i=0;i<total;i++) {
                learned_means[j] += responsibility[i*num_components+j] * data[i];
            }

            if(nc[j] > 0) {
                learned_means[j] /= nc[j];
            }

            for(int i=0;i<total;i++) {
                float diff = data[i] - learned_means[j];
                learned_std[j] += responsibility[i*num_components+j] * diff * diff;
            }

            if(nc[j] > 0) {
                learned_std[j] /= nc[j];
            }

            learned_std[j] = sqrt(learned_std[j]);
        }

        const int graph_width = 640;
        const int graph_height = 140;
        cv::Mat graph(graph_height, graph_width, CV_8UC3, cv::Scalar(255, 255, 255));
        draw_1d_data_mixture(graph, data, responsibility, count_per_component, num_components);
        draw_1d_gaussian_mixture(graph, learned_means, learned_std, pi, num_components);

        printf("Iteration %04d\n", iter);
        for(int j=0;j<num_components;j++) {

            printf("Learned #%d - mean = %f, std = %f, pi =%f\n", j, learned_means[j], learned_std[j], pi[j]);
        }

        char file[100];
        sprintf(file, "./mixture-gaussian-iter%02d.png", iter);
        cv::imwrite(file, graph);
    }

}

int main(int argc, char* argv[]) {
    run_gaussian_mixture_1d();
    return 0;
}
