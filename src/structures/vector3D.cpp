#include "vector3D.h"
#include <stdexcept>

double& vector3D::operator[](const size_t i) {
    if(i > 2)
        throw std::out_of_range("vector3D: out of range");
    return _crdVec[i];
}
const double& vector3D::operator[](const size_t i) const {
    if(i > 2)
        throw std::out_of_range("vector3D: out of range");
    return _crdVec[i];
}

//map() function is analogy for std::transform in java/python

vector3D operator+(const vector3D& lhs, const vector3D& rhs) {
    vector3D res;
    std::transform(lhs._crdVec.cbegin(), lhs._crdVec.cend(), rhs._crdVec.cbegin(), res._crdVec.begin(), std::plus<>());
    return res;
}

vector3D operator-(const vector3D& lhs, const vector3D& rhs) {
    vector3D res;
    std::transform(lhs._crdVec.cbegin(), lhs._crdVec.cend(), rhs._crdVec.cbegin(), res._crdVec.begin(), std::minus<>());
    return res;
}

vector3D operator*(double lhs, const vector3D& rhs) {
    vector3D res;
    std::transform(rhs._crdVec.cbegin(), rhs._crdVec.cend(), res._crdVec.begin(), [lhs](double x) { return lhs * x; });
    return res;
}

//double operator*(const vector3D& lhs, const vector3D& rhs) {
//    double res = 0.0;
//    for(size_t i = 0; i < 3; ++i)
//        res += lhs[i] * rhs[i];
//    return res;
//}

//bool operator==(const vector3D& lhs, const vector3D& rhs) {
//    bool res = true;
//    for(size_t i = 0; i < lhs._crdVec.size(); ++i) {
//        if(lhs._crdVec[i] != rhs._crdVec[i])
//            res = false;
//    }
//    return (res && lhs.equalLength(rhs));
//}
//
//std::ostream& operator<<(std::ostream& os, const vector3D& vec) {
//    for(const auto& it : vec._crdVec)
//        os << it << " ";
//    os << std::endl;
//    return os;
//}

