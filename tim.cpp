#include <iostream>
#include <fstream>
#include <cstdlib>

int main(int argc, char *argv[]) {
    std::ifstream in(argv[1]);
    std::ofstream out(argv[2], std::ios::app);
    int mode = atoi(argv[3]);
    double tmp, sum;
    tmp = sum = 0;
    int i = 0;
    in >> tmp;
    while(!in.eof()) {
        sum += tmp;
        ++i;
        in >> tmp;
    }   
    if(i) {
        sum /= i;
    }
    out << mode << ' ' << sum <<std::endl;
    in.close();
    out.close();
    return 0;
}       
