#include <iostream>
#include <fstream>
#include <vector>

#include <memory>

#include <deal.II/lac/sparsity_pattern.h>
#include <deal.II/lac/dynamic_sparsity_pattern.h>

using namespace std;
using namespace dealii;

DynamicSparsityPattern from_static_to_dynamic(SparsityPattern & sp){
    DynamicSparsityPattern dsp(sp.n_rows(),sp.n_cols());

    for (auto & it : sp)
        dsp.add(it.row(),it.column());

    return dsp;
}

int main(){

    DynamicSparsityPattern dsp(5,5);

    dsp.add(3,2);

    shared_ptr<SparsityPattern> sp(new SparsityPattern());

    sp->copy_from(dsp);
    sp->compress();

    ofstream myfile;
    myfile.open ("sp.svg");
    sp->print_svg(myfile);
    myfile.close();

    vector<shared_ptr<SparsityPattern>> vec_sp;

    vec_sp.push_back(sp);
    cout << "vec_sp[0] = " << vec_sp[0]->n_nonzero_elements() << endl;

    DynamicSparsityPattern dsp2 = from_static_to_dynamic(*sp);
    dsp2.symmetrize();

    shared_ptr<SparsityPattern> sp1(new SparsityPattern());
    sp1->copy_from(dsp2);

    myfile.open ("sym_sp.svg");
    sp1->print_svg(myfile);
    myfile.close();

    vec_sp.push_back(sp1);

    cout << "vec_sp[1] = " << vec_sp[1]->n_nonzero_elements() << endl;

    cout << "stocazzo" << endl;

    return 0;
}
