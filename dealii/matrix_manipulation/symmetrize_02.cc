#include <iostream>
#include <fstream>
#include <vector>

#include <memory>

#include <deal.II/lac/sparsity_pattern.h>

using namespace std;
using namespace dealii;

void print_sp(const SparsityPattern &sp, string filename){
    ofstream myfile;
    myfile.open (filename);
    sp.print_svg(myfile);
    myfile.close();
}

SparsityPattern create_tridiag_sp(const size_t n){
    SparsityPattern sp(n,n,3);

    for (size_t i = 0; i < n; i++)
        sp.add(i,i);

    for (size_t i = 0; i < n-1; i++)
        sp.add(i,i+1);

    for (size_t i = 0; i < n-1; i++)
        sp.add(i+1,i);

    return sp;
}

SparsityPattern upper_tri_band(const size_t n){
    SparsityPattern sp(n,n,3);

    for (size_t i = 0; i < n; i++)
        sp.add(i,i);

    for (size_t i = 0; i < n-2; i++)
        sp.add(i,i+2);

    return sp;
}


int main(){

    size_t n = 10;

    SparsityPattern sp1 = upper_tri_band(n);

    sp1.compress();

    print_sp(sp1,"sp1.svg");

    size_t entries = sp1.max_entries_per_row();
    cout << "max_entries_per_row = " << entries << endl;

    SparsityPattern sp2(sp1,entries*3,entries);

    sp2.symmetrize();

    sp2.compress();

    print_sp(sp2,"sp2.svg");

    return 0;
}
