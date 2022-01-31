#include <iostream>

using namespace std;

void swap(int * p_one, int * p_two){

  int tmp;

  tmp = *p_two;

  *p_two = *p_one;
  *p_one = tmp;

}

int main(){
  cout << "stocazzo" << endl;

  int one = 1;
  int two = 2;

  int * p_one  = &one;
  int * p_two  = &two;

  cout << "one =" << *p_one << endl;
  cout << "two =" << *p_two << endl;

  swap(p_one,p_two);

  cout << "one =" << *p_one << endl;
  cout << "two =" << *p_two << endl;

  return 0;
}
