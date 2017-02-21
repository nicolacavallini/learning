#include "iostream"
#include <sys/stat.h>

using namespace std;

struct stat sb;


int main(){
    cout << "stocazzo" << endl;
    string pathname = "foo";
    //char pn = *pathname.c_str();
    if (stat(pathname.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)){

        cout << "The directory exists" << endl;

    }

    const int dir_err = mkdir(pathname.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (-1 == dir_err){
    cout << "Error creating directory!" <<endl;
    }
    return 0;
}
