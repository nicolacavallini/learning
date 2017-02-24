#include <iostream>
#include <memory>

#include <deal.II/lac/vector.h>
#include <deal.II/lac/vector.templates.h>

using namespace std;
using namespace dealii;

template class Vector<int>;

template< class T>
class GLCM{
public:
    GLCM() : v(new Vector<T>()){}

    GLCM(unsigned int in) : v(new Vector<T>()){
        v->reinit(in);
        *v = 1;
    }
    void print_info(){
        v->print(cout);
    }
    shared_ptr<Vector<T>> get_v(){
        return v;
    }
protected:
    shared_ptr<Vector<T>> v;
};

template< class T>
class NormalizedGLCM : public GLCM<double>{
public:
    NormalizedGLCM(GLCM<T> &glcm) {
        auto tmp = glcm.get_v();
        double somma = tmp->norm_sqr();
        //*tmp /= somma;
        v.reset(new Vector<double>(*tmp));
        *v /= somma;
    }
};

template class GLCM<int>;
template class NormalizedGLCM<int>;

int main(){
    GLCM<int> glcm(5);
    glcm.print_info();

    NormalizedGLCM<int> n_glcm(glcm);
    n_glcm.print_info();




    return 0;
}
