#include<string>
#include<iostream>
int main()
{
    const std::string path="symm_norm";
    auto const pos=path.find_last_of('_');
    const auto leaf=path.substr(pos+1);

    std::cout << leaf << '\n';
}
