#include <iostream>
#include <fstream>

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
    cout << "stocazzo" << endl;
    DynamicSparsityPattern dsp(5,5);

    dsp.add(3,2);


    SparsityPattern sp;
    sp.copy_from(dsp);


    ofstream myfile;
    myfile.open ("sp.svg");
    sp.print_svg(myfile);
    myfile.close();

    DynamicSparsityPattern dsp2 = from_static_to_dynamic(sp);

    dsp2.symmetrize();
    sp.copy_from(dsp2);

    myfile.open ("sym_sp.svg");
    sp.print_svg(myfile);
    myfile.close();

    return 0;
}
