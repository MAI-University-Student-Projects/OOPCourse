#include <iostream>
#include "structures/vector3D.h"

int main(int argc, char* argv[]) {
    vector3D vec_ma(0, 0, 0);
    std::cout << ((vec_ma + vector3D(0,0,1))  == vector3D(0,0,1)) << std::endl;
    return 0;
}
