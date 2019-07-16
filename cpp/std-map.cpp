#include <iostream>
#include <map>
#include <unordered_map>
#include <string>

using namespace std;

int main(){

  map<string,string> data;

  data.insert(pair<string,string>("nicola","italy"));
  data.insert(pair<string,string>("carlos","spain"));

  data.erase("nicola");


  unordered_map<string,string> ud;
  ud.insert(pair<string,string>("nicola","italy"));

  ud.erase("nicola");


  return 0;
}
