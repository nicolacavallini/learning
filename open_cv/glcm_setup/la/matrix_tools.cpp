#include "matrix_tools.h"

#include "../assert.h"


template<class input_format, class output_format>
void print_matrix(const Mat_<input_format> &P)
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

template<typename T>
Mat_<T> create_i_long_column(int l)
{
    Mat_<T> i_column(l,1);

    for (int i =0; i<l; i++)
        i_column(i,0) = (T)i;

    return i_column;
}

template<typename T>
Mat_<T> create_j_long_row(int l)
{
    Mat_<T> j_row(1,l);

    for (int j =0; j<l; j++)
        j_row(0,j) = (T)j;

    return j_row;
}

template<class T>
T scalar_prod(Mat_<T> &row, Mat_<T> &col)
{
    ASSERT(row.rows==1, "this is not a row, num cols!= 1");
    ASSERT(col.cols==1, "this is not a column, num rows!= 1");
    ASSERT(row.cols==col.rows, "first matrix cols, not equal second matrix rows");

    T acc = 0;

    auto ir = row.begin();
    auto ic = col.begin();
    for (; ir != row.end() ; ++ir, ++ic)
        acc+= *ir * *ic;

    return acc;
}


template<class T>
Mat_<T> mat_mat_mult(Mat_<T> &row, Mat_<T> &col)
{
    ASSERT(row.cols==col.rows, "first matrix cols, not equal second matrix rows");

    int i = row.rows;
    int j = col.cols;

    Mat_<T> result(i,j);

    for (int id = 0; id < i ; id++){
        Mat_<T> r = row.row(id);
        for (int jd = 0; jd < j ; jd++){
            Mat_<T> c = col.col(jd);
            result(id,jd) = scalar_prod(r,c);
        }
    }
    //gemm(col,row,1.,z,1.,result);
    return result;
}

template<class T>
Mat_<T> mult_row_by_i(const Mat_<T> &row)
{
    int i = row.rows;
    int j = row.cols;

    Mat_<T> result(i,j);

    for (int id = 0; id < i ; id++)
        for (int jd = 0; jd < j ; jd++)
            result(id,jd) = row(id,jd) * id;

    //gemm(col,row,1.,z,1.,result);
    return result;
}

template<class T>
Mat_<T> mult_col_by_j(const Mat_<T> &row)
{
    int i = row.rows;
    int j = row.cols;

    Mat_<T> result(i,j);

    for (int id = 0; id < i ; id++)
        for (int jd = 0; jd < j ; jd++)
            result(id,jd) = row(id,jd) * jd;

    return result;
}

template<class T>
Mat_<T> elem_elem_prod(Mat_<T> &row, Mat_<T> &col)
{
    ASSERT(row.rows==col.rows, "the two arrays dimensions have to be the same");
    ASSERT(row.cols==col.cols, "the two arrays dimensions have to be the same");

    int i = row.rows;
    int j = col.cols;

    Mat_<T> result(i,j);

    for (int id = 0; id < i ; id++)
        for (int jd = 0; jd < j ; jd++)
            result(id,jd) = row(id,jd)*col(id,jd);

    return result;
}

template<class T>
Mat_<T> shif_by_scalar(Mat_<T> &row, T shift, bool negative=true)
{
    T sign = 1;
    if (negative)
        sign = -1;

    int i = row.rows;
    int j = row.cols;

    Mat_<T> result(i,j);

    for (int id = 0; id < i ; id++)
        for (int jd = 0; jd < j ; jd++)
            result(id,jd) = row(id,jd) + sign * shift;

    return result;
}

vector<vector<bool>> mask_zero_std(vector<double> &std_i ,
                                   vector<double> &std_j)
{
    vector<vector<bool>> mask;

    auto ir = std_i.begin();
    for (; ir != std_i.end(); ++ir){
        auto ic = std_j.begin();

        vector<bool> row;
        for (; ic != std_j.end(); ++ic){
            //cout << *ir << ", " << *ic << endl;
            if(*ir < 1e-15 || *ic < 1e-15)
                row.push_back(false);
            else
                row.push_back(true);
        }
        mask.push_back(row);
    }
    return mask;
}

template<class T>
T sum_ij(const Mat_<T> &image)
{
    return (T)accumulate(image.begin(),
                              image.end(), 0.0);
}

template void print_matrix<int,int>(const Mat_<int> &P);

template Mat_<int> create_i_long_column<int>(int l);
template Mat_<int> create_j_long_row<int>(int l);
template int scalar_prod(Mat_<int> &row, Mat_<int> &col);
template Mat_<int> mat_mat_mult(Mat_<int> &row, Mat_<int> &col);

template Mat_<int> elem_elem_prod(Mat_<int> &row, Mat_<int> &col);
template Mat_<int> shif_by_scalar(Mat_<int> &row,
                                  int shift, bool negative=true);

template Mat_<int> mult_row_by_i(const Mat_<int> &row);
template Mat_<int> mult_col_by_j(const Mat_<int> &row);

template Mat_<uint8_t> create_i_long_column<uint8_t>(int l);
template Mat_<uint8_t> create_j_long_row<uint8_t>(int l);

template int sum_ij(const Mat_<int> &image);
