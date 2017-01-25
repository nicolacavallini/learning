#ifndef TOOLS_H
#define TOOLS_H

#include <cv.h>

#include <iostream>

#include <math.h>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

using namespace std;
using namespace cv;


/*double sum = std::accumulate(v.begin(), v.end(), 0.0);
double mean = sum / v.size();

double sq_sum = std::inner_product(v.begin(), v.end(), v.begin(), 0.0);
double stdev = std::sqrt(sq_sum / v.size() - mean * mean);*/

template<class T>
Mat_<T> gery_co_matrix(const Mat_<T> &image,
                    const double &distance,
                    const double &angle,
                    const T &levels)
{
    Mat_<T> output = Mat_<T>::zeros(levels,
                            levels);

    int delta_r = (int)round(sin(angle)*distance);
    int delta_c = (int)round(cos(angle)*distance);

    for (int r = 0; r< levels; r++){
        for (int c =0; c < levels; c++){

            T i = image(r,c);

            int d_row = r + delta_r;
            int d_col = c + delta_c;

            if (d_row >= 0 && d_row < levels &&
                d_col >= 0 && d_col < levels ){

                T j = image(d_row,d_col);

                if (i >= 0 && i < levels &&
                    j >= 0 && j < levels){

                    output(i,j)+=1;
                }
            }
        }
    }

    return output;
}

template<class T>
void explore_matrix(const Mat_<T> &image)
{
    int count= 0;
    auto it = image.begin();
    for (; it != image.end() ; ++it)
    {
        int i = floor ((double)count / (double)image.cols);
        int j = count - image.cols*i;
        cout << "count = " << count;
        cout << ", i = " << i;
        cout << ", j = " << j;
        cout << ", element = " << *it << endl;
        count++;
    }
}

/*double wheight_energy()
{
    return 1.;
}*/

template<class T>
double evaluate_energy(const Mat_<T> &image)
{
    double energy = 0;

    auto it = image.begin();
    for (; it != image.end() ; ++it)
        energy+= (double)*it * (double)*it;

    return sqrt(energy);
}

template<class T>
double evaluate_contrast(const Mat_<T> &image)
{
    double entropy = 0;

    int count= 0;
    auto it = image.begin();
    for (; it != image.end() ; ++it)
    {
        int i = floor ((double)count / (double)image.cols);
        int j = count - image.cols*i;
        //cout << "count = " << count;
        //cout << ", i = " << i;
        //cout << ", j = " << j;
        //cout << ", element = " << *it;
        //cout << ", pow((i-j),2) = " << pow((i-j),2) << endl;
        entropy += pow((i-j),2)* (double)*it;
        count++;
    }
    return entropy;
}

template<class T>
double evaluate_homogeneity(const Mat_<T> &image)
{
    double entropy = 0;

    int count= 0;
    auto it = image.begin();
    for (; it != image.end() ; ++it)
    {
        int i = floor ((double)count / (double)image.cols);
        int j = count - image.cols*i;
        //cout << "count = " << count;
        //cout << ", i = " << i;
        //cout << ", j = " << j;
        //cout << ", element = " << *it;
        //cout << ", pow((i-j),2) = " << pow((i-j),2) << endl;
        entropy += 1./(pow((i-j),2)+1)* (double)*it;
        count++;
    }
    return entropy;
}

template<class T>
double evaluate_dissimilarity(const Mat_<T> &image)
{
    double entropy = 0;

    int count= 0;
    auto it = image.begin();
    for (; it != image.end() ; ++it)
    {
        int i = floor ((double)count / (double)image.cols);
        int j = count - image.cols*i;
        //cout << "count = " << count;
        //cout << ", i = " << i;
        //cout << ", j = " << j;
        //cout << ", element = " << *it;
        //cout << ", pow((i-j),2) = " << pow((i-j),2) << endl;
        entropy += abs(i-j)* (double)*it;
        count++;
    }
    return entropy;
}

double weight_correlation(int &i, int &j,
                          vector<double> &mean_i,
                          vector<double> &std_i,
                          vector<double> &mean_j,
                          vector<double> &std_j)
{
    //cout << "mean_i["<< i  << "] = "
    //     << mean_i[i] << endl;

    //cout << "j = " << j << "/" <<
    //        mean_j.size() << endl;
    //cout << "mean_j["<< j  << "] = "
    //     << mean_j[j] << endl;
    double num = ((double)i - mean_i[i]) *
                 ((double)j - mean_j[j]);

    double den = std_i[i]* std_j[j];

    cout << "num = " << num <<
            ", den = " << den << endl;

    return num/den;
}

template<class T>
double evaluate_glcm_mean(Mat_<T> &image, int &i)
{
    double entropy = 0;

    auto it = image.begin();
    for (; it != image.end() ; ++it)
        entropy += (double)i * (double)*it;

    return entropy;
}

template<class T>
double evaluate_glcm_stdv(Mat_<T> &image, int &i)
{

    double mu_i = evaluate_glcm_mean(image,i);

    double entropy = 0;

    auto it = image.begin();
    for (; it != image.end() ; ++it)
        entropy += pow((double)i-mu_i,2) * (double)*it;

    return entropy;
}

vector<vector<bool>> mask_zero_std(vector<double> &std_i ,
                                   vector<double> &std_j)
{
    /* This has to become a function */
    /* if correlation call the function */
    vector<vector<bool>> mask;

    auto ir = std_i.begin();
    for (; ir != std_i.end(); ++ir){
        auto ic = std_j.begin();

        vector<bool> row;
        for (; ic != std_j.end(); ++ic){
            cout << *ir << ", " << *ic << endl;
            if(*ir < 1e-15 || *ic < 1e-15)
                row.push_back(false);
            else
                row.push_back(true);
        }
        mask.push_back(row);
    }
    return mask;
}

template<class  T>
double evaluate_correlation(const Mat_<T> &image)
{
    vector<double> mean_i;
    vector<double> std_i;

    vector<double> mean_j;
    vector<double> std_j;

    cout << image <<endl;

    for (int i=0; i <  image.rows; i ++){

        Mat_<T> row = image.row(i);

        mean_i.push_back(
                    evaluate_glcm_mean(row,i));

        std_i.push_back(evaluate_glcm_stdv(row,i));
    }

    for (int j=0; j <  image.cols; j ++){

        Mat_<T> col = image.col(j);

        mean_j.push_back(
                    evaluate_glcm_mean(col,j));

        std_j.push_back(evaluate_glcm_stdv(col,j));
    }



    double entropy = 0;

    int count= 0;
    auto it = image.begin();
    for (; it != image.end() ; ++it)
    {
        int i = floor ((double)count / (double)image.cols);
        int j = count - image.cols*i;
        entropy += weight_correlation(i,j,
                                      mean_i,std_i,
                                      mean_j,std_j)
                * (double)*it;
        count++;
    }
    return entropy;
}

template<class T>
double evaluate_entropy(const Mat_<T> &image)
{
    double entropy = 0;

    auto it = image.begin();
    for (; it != image.end() ; ++it)
        entropy+= (double)*it * log2((double)*it);

    return -1.*entropy;
}

template<class T>
Mat_<double> normalise_co_matrix(const Mat_<T> &image)
{
    double acc = 0;
    auto it = image.begin();
    for (; it != image.end() ; ++it)
        acc+= (double)*it;

    Mat_<double> n(image.rows,image.cols);

    int count = 0;

    it = image.begin();
    for (; it != image.end() ; ++it){
        int i = floor ((double)count / (double)image.cols);
        int j = count - image.cols*i;
        n(i,j) = (double)*it/acc;
        count++;
    }
    return n;
}

template<class T>
Mat_<T> simmetrise_co_matrix( Mat_<T> &image)
{
    Mat_<T> n = image.clone();

    for (int i = 0; i<image.rows; i++)
        for (int j = 0; j<image.cols; j++)
            n(i,j)+=image(j,i);

    return n;
}

template<class input_format, class output_format>
void print_matrix(Mat_<input_format> &P)
{
    std::cout.precision(15);
    cout << "[";
    for (int i =0; i < P.rows; i++)
    {
        if (i!=0) cout << " ";
        for (int j =0; j < P.cols; j++)
        {
            cout << (output_format)P(i,j);
            if (j == P.cols-1 && i !=P.rows-1)
                cout << endl;
            else if (j == P.cols-1 && i ==P.rows-1)
                break;
            else
                cout << ", ";
        }
    }
    cout << "]" << endl;
}

#endif // TOOLS_H
