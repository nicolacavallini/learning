#include <iostream>
#include <array>
#include <functional>
#include <cassert>

#include <Eigen/Dense>
//using Eigen;

#include <cmath>

typedef std::array<double,3> a3;
typedef std::function<double(const a3 &)> f_a3;

/**
 * F = R*n1 - n2
 */
template<int dim>
std::array<std::function<double(const std::array<double,dim> &)>,dim>
                                   constuct_function(const std::array<double,dim> &n1,
                                                     const std::array<double,dim> &n2);


/**
 * F = R*n1 - n2
 */
template<>
std::array<f_a3,3> constuct_function<3>(const a3 &n1, const a3 &n2){

  double k = n1[0], m = n1[1], n = n1[2];
  double r = n2[0], s = n2[1], t = n2[2];

  std::array<f_a3,3>  out;

  out [0] = [k,m,n,r,s,t](const a3 &a){return k*(std::cos(a[1]) + std::cos(a[2]) + 1) - m*std::sin(a[2]) + n*std::sin(a[1]) - r;};
  out [1] = [k,m,n,r,s,t](const a3 &a){return k*std::sin(a[2]) + m*(std::cos(a[0]) + std::cos(a[2]) + 1) - n*std::sin(a[0]) - s;};
  out [2] = [k,m,n,r,s,t](const a3 &a){return -k*std::sin(a[1]) + m*std::sin(a[0]) + n*(std::cos(a[0]) + std::cos(a[1]) + 1) - t;};


  return out;
}

template<int dim>
std::array<std::array<std::function<double(const std::array<double,dim> &)>,dim>,dim>
                                   constuct_jacobian(const std::array<double,dim> &n1,
                                                     const std::array<double,dim> &n2);

template<>
std::array<std::array<f_a3,3>,3> constuct_jacobian<3>(const a3 &n1,
                                                      const a3 &n2){

  std::array<std::array<f_a3,3>,3> out;

  double k = n1[0], m = n1[1], n = n1[2];
  double r = n2[0], s = n2[1], t = n2[2];


  out[0][0] =  [k,m,n,r,s,t](const a3 &a){return 0;};
  out[0][1] =  [k,m,n,r,s,t](const a3 &a){return -k*sin(a[1]) + n*cos(a[1]);};
  out[0][2] =  [k,m,n,r,s,t](const a3 &a){return -k*sin(a[2]) - m*cos(a[2]);};
  out[1][0] =  [k,m,n,r,s,t](const a3 &a){return -m*sin(a[0]) - n*cos(a[0]);};
  out[1][1] =  [k,m,n,r,s,t](const a3 &a){return 0;};
  out[1][2] =  [k,m,n,r,s,t](const a3 &a){return k*cos(a[2]) - m*sin(a[2]);};
  out[2][0] =  [k,m,n,r,s,t](const a3 &a){return m*cos(a[0]) - n*sin(a[0]);};
  out[2][1] =  [k,m,n,r,s,t](const a3 &a){return -k*cos(a[1]) - n*sin(a[1]);};
  out[2][2] =  [k,m,n,r,s,t](const a3 &a){return 0;};

  return out;
}


template<int dim>
Eigen::VectorXd constuct_function_vector(const std::array<std::function<double(const std::array<double,dim> &)>,dim> &func, 
                                         const Eigen::VectorXd &in){

  assert(in.size()==dim);

  Eigen::VectorXd out(dim);

  std::array<double,dim> ain;

  for (int i = 0; i <dim; i++)
    ain[i] = in(i);

  for (int i = 0; i <dim; i++)
    out(i) = func[i](ain);

  return out;
}

template<int dim>
Eigen::MatrixXd constuct_jacobian_matrix(const std::array<std::array<std::function<double(const std::array<double,dim> &a)>,dim>,dim> &jac, 
                                         const Eigen::VectorXd &in){
  Eigen::MatrixXd out(dim,dim);

  std::array<double,dim> ain;

  for (int i = 0; i <dim; i++)
    ain[i] = in(i);

  for (int i = 0; i <dim; i++)
    for (int j = 0; j <dim; j++)
      out(i,j) = jac[i][j](ain);
    
  return out;
}



int main()
{

  const int dim = 3;

  const std::array<double,dim> n1 = {0.28571429,0.85714286,0.42857143};
  const std::array<double,dim> n2 = {0.44597649,0.66896473,0.59463532};
  
  std::array<std::function<double(const std::array<double,dim> &a)>,dim> func = constuct_function<dim>(n1,n2);

  std::array<std::array<std::function<double(const std::array<double,dim> &a)>,dim>,dim> jac = constuct_jacobian<dim>(n1,n2);


  const std::array<double,dim> angle = {0.44597649,0.66896473,0.59463532};

  Eigen::VectorXd angle_e(dim);

  for (int i = 0; i <dim; i++)
    angle_e(i) = angle[i];

  Eigen::VectorXd fv = constuct_function_vector<dim>(func,angle_e);

  for (int i = 0; i <dim; i++)
    std::cout << "f[" << i << "] = " << func[i](angle) << std::endl;

  for (int i = 0; i <dim; i++)
    for (int j = 0; j <dim; j++)
      std::cout << "jac[" << i << "][" << j << "] = " << jac[i][j](angle) << std::endl;
  
  std::cout << "fv = " << fv << std::endl;

  Eigen::MatrixXd  jac_m = constuct_jacobian_matrix<3>(jac,angle_e);

  std::cout << "jac_m = " << jac_m << std::endl;
}