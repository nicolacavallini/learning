#include <iostream>

#include <deal.II/base/logstream.h>

#include <deal.II/lac/full_matrix.h>
#include <deal.II/lac/full_matrix.templates.h>

using namespace dealii;

template class FullMatrix<int>;

using namespace std;

template <typename number>
void
print_matrix (const FullMatrix<number > &m)
{
  deallog.depth_console (2);
  for (unsigned int i=0; i<m.m(); ++i)
    for (unsigned int j=0; j<m.n(); ++j)
      deallog << i << ' ' << j << ' ' << m(i,j)
              << std::endl;
}

int main()
{
    FullMatrix<int> m;
    m.reinit(5,5);

    print_matrix(m);

    cout << "stocazzo" << endl;
    return 0;
}
