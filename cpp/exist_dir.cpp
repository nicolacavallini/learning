#include <sys/stat.h>

#include <iostream>
#include <cstdlib>

using namespace std;

bool if_directory_exists(std::string filename)
{
  bool esiste = false;
  struct stat sb;
  if (stat(filename.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
  {
    esiste = true;
  }
  return esiste;
}

int main(int argc, char* argv[])
{
  if (if_directory_exists("/home/nicola/"))
      cout << "the directory exists" << endl ;
  else
      cout << "the does not directory exists" << endl ;
}
