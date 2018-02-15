#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cctype>
 
int main() 
{
    // remove duplicate elements (normal use)
    std::vector<int> v{1,2,3,1,2,3,3,4,5,4,5,6,7};
    std::sort(v.begin(), v.end()); // 1 1 2 2 3 3 3 4 4 5 5 6 7 
    auto last = std::unique(v.begin(), v.end());
    // v now holds {1 2 3 4 5 6 7 x x x x x x}, where 'x' is indeterminate
    v.erase(last, v.end()); 
    for (int i : v)
      std::cout << i << " ";
    std::cout << "\n";
 
    // remove consecutive spaces
    std::string s = "wanna go    to      space?";
    auto end = std::unique(s.begin(), s.end(), [](char l, char r){
        return std::isspace(l) && std::isspace(r) && l == r;
    });
    // s now holds "wanna go to space?xxxxxxxx", where 'x' is indeterminate
    std::cout << std::string(s.begin(), end) << '\n';
}
