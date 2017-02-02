#include "analysis_tools.h"

#include "../assert.h"
#include "../la/matrix_tools.h"

template<class T>
Mat_<T> gery_co_matrix(const Mat_<T> &image,
                    const double distance,
                    const double angle,
                    const int levels)
{
    Mat_<T> output = Mat_<T>::zeros(levels,
                            levels);

    int delta_r = (int)round(sin(angle)*distance);
    int delta_c = (int)round(cos(angle)*distance);

    for (int r = 0; r< image.rows; r++){
        for (int c =0; c < image.cols; c++){

            int i = (int)image(r,c);

            int d_row = r + delta_r;
            int d_col = c + delta_c;

            if (d_row >= 0 && d_row < image.rows &&
                d_col >= 0 && d_col < image.cols ){

                int j = (int)image(d_row,d_col);

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

template<class  T>
Mat_<T> evaluate_diff(Mat_<T> &image, string dir = "i")
{
    ASSERT(dir == "i" || dir == "j",
    "the direction can be eather the row 'i' or the column `i`");

    int levels = image.rows;

    Mat_<T> i, pi;

    if(dir=="i"){
        i = create_i_long_column<T>(levels);
        pi = mult_row_by_i(image);
    }
    else if(dir=="j"){
        i = create_j_long_row<T>(levels);
        pi = mult_col_by_j(image);
    }

    T spi = sum_ij(pi);
    return shif_by_scalar(i,spi);
}

template<class  T>
double evaluate_std(Mat_<T> &image, string dir = "i")
{
    ASSERT(dir == "i" || dir == "j",
    "the direction can be eather the row 'i' or the column `i`");

    Mat_<T> diff_i = evaluate_diff(image,dir);

    Mat_<T> di = elem_elem_prod(diff_i,diff_i);

    Mat_<T> tmp;

    if(dir=="i"){
        tmp = Mat_<T>::ones(1,image.cols);
        di = mat_mat_mult(di,tmp);
    }
    else if(dir=="j"){
        tmp = Mat_<T>::ones(image.rows,1);
        di = mat_mat_mult(tmp,di);
    }

    Mat_<T> s_i = elem_elem_prod(image,di);

    return sqrt((double)sum_ij(s_i));
}

template<class  T>
T evaluate_covariance(Mat_<T> &image)
{
    Mat_<T> diff_i = evaluate_diff(image,"i");
    Mat_<T> diff_j = evaluate_diff(image,"j");

    Mat_<T> diff = mat_mat_mult(diff_i,diff_j);

    Mat_<T> c = elem_elem_prod(image,diff);

    return sum_ij(c);
}

template<class  T>
double evaluate_correlation(Mat_<T> &image)
{
    double std_i = evaluate_std(image,"i");

    double std_j  = evaluate_std(image,"j");

    T cov = evaluate_covariance(image);

    double correlation = ((double)cov)/(std_i*std_j);
    return correlation;
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


template Mat_<int> gery_co_matrix(const Mat_<int> &image,
                    const double distance,
                    const double angle,
                    const int levels);

template Mat_<uint16_t> gery_co_matrix(const Mat_<uint16_t> &image,
                    const double distance,
                    const double angle,
                    const int levels);


template void explore_matrix(const Mat_<int> &image);
template double evaluate_energy(const Mat_<int> &image);
template double evaluate_energy(const Mat_<double> &image);
template double evaluate_contrast(const Mat_<int> &image);
template double evaluate_homogeneity(const Mat_<int> &image);
template double evaluate_dissimilarity(const Mat_<int> &image);
template double evaluate_dissimilarity(const Mat_<double> &image);
template Mat_<int> evaluate_diff(Mat_<int> &image, string dir = "i");
template double evaluate_std(Mat_<int> &image, string dir = "i");
template int evaluate_covariance(Mat_<int> &image);
template double evaluate_correlation(Mat_<int> &image);
template double evaluate_correlation(Mat_<double> &image);
template double evaluate_entropy(const Mat_<int> &image);
template Mat_<double> normalise_co_matrix(const Mat_<int> &image);
template Mat_<double> normalise_co_matrix(const Mat_<double> &image);
template Mat_<int> simmetrise_co_matrix( Mat_<int> &image);
template Mat_<double> simmetrise_co_matrix( Mat_<double> &image);
