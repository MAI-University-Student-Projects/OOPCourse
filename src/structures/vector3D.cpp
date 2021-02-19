#include "vector3D.h"
#include <stdexcept>

vector3D& vector3D::operator=(const vector3D& oth) {
    if(this != &oth)
        std::copy(oth._crdVec, oth._crdVec + 3, _crdVec);
    return *this;
}

vector3D& vector3D::operator+=(const vector3D& oth) {
    for(size_t i = 0; i < 3; ++i)
        _crdVec[i] += oth[i];
    return *this;
}
vector3D& vector3D::operator-=(const vector3D& oth) {
    for(size_t i = 0; i < 3; ++i)
        _crdVec[i] -= oth[i];
    return *this;
}
vector3D& vector3D::operator*=(double cf) {
    for(size_t i = 0; i < 3; ++i)
        _crdVec[i] *= cf;
    return *this;
}

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
    for(size_t i = 0; i < 3; ++i)
        res._crdVec[i] = lhs._crdVec[i] + rhs._crdVec[i];
    return res;
}

vector3D operator-(const vector3D& lhs, const vector3D& rhs) {
    vector3D res;
    for(size_t i = 0; i < 3; ++i)
        res._crdVec[i] = lhs._crdVec[i] - rhs._crdVec[i];
    return res;
}

vector3D operator*(double lhs, const vector3D& rhs) {
    vector3D res;
    for(size_t i = 0; i < 3; ++i)
        res._crdVec[i] = lhs * rhs._crdVec[i];
    return res;
}

double operator*(const vector3D& lhs, const vector3D& rhs) {
    double res = 0.0;
    for(size_t i = 0; i < 3; ++i)
        res += lhs._crdVec[i] * rhs._crdVec[i];
    return res;
}

bool operator==(const vector3D& lhs, const vector3D& rhs) {
    bool res = true;
    for(size_t i = 0; i < 3; ++i) {
        res = (lhs._crdVec[i] == rhs._crdVec[i]);
        if(!res) break;
    }
    return res;
}
