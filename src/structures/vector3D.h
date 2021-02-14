#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <iostream>
#include <algorithm>
#include <cmath>
#include <array>
#include <functional>
#include <numeric>

class vector3D {
public:
    vector3D() : vector3D(0.0, 0.0, 0.0) { }
    explicit vector3D(double x, double y, double z) : _crdVec{x, y, z} {}
    explicit vector3D(const std::array<double, 3>& arr_3d) : _crdVec(arr_3d) {}
    
    vector3D(const vector3D& oth) {
        std::copy(oth._crdVec.cbegin(), oth._crdVec.cend(), _crdVec.begin());
    }
    vector3D(vector3D&& oth) : vector3D() {
        swap(*this, oth);
    }
    
    /* with lvalue: copy-constructed in argument
     with rvalue: move-constructed in argument */
    vector3D& operator=(vector3D oth) {
        swap(*this, oth);
        return *this;
    }
    friend void swap(vector3D& vec_frst, vector3D& vec_scnd) {
        std::swap(vec_frst._crdVec, vec_scnd._crdVec);
    }
    
    double& operator[](const size_t i);
    const double& operator[](const size_t i) const;
    vector3D& operator+=(const vector3D& oth) {
        std::transform(_crdVec.cbegin(), _crdVec.cend(), oth._crdVec.begin(), _crdVec.begin(), std::plus<>());
        return *this;
    }
    vector3D& operator-=(const vector3D& oth) {
        std::transform(_crdVec.cbegin(), _crdVec.cend(), oth._crdVec.begin(), _crdVec.begin(), std::minus<>());
        return *this;
    }
    vector3D& operator*=(double cf) {
        std::transform(_crdVec.cbegin(), _crdVec.cend(), _crdVec.begin(), [cf](double x) { return cf * x;});
        return *this;
    }
    
    friend vector3D operator+(const vector3D& lhs, const vector3D& rhs);
    friend vector3D operator-(const vector3D& lhs, const vector3D& rhs);
    friend vector3D operator*(double lhs, const vector3D& rhs);
    
    // _Tp __init - 4-th arg in inner_product, takes initial value to sum elements of structure with
    friend double operator*(const vector3D& lhs, const vector3D& rhs) {
        return std::inner_product(lhs._crdVec.cbegin(), lhs._crdVec.cend(), rhs._crdVec.cbegin(), 0.0);
    }
    // std::transform_reduce (c++17) - parallelized version for std::inner_product ar std::reduce() for std::accumulate
    // first arg - execution policy, could stay empty (parallel policy) or std::execution::seq(sequenced) or any other policy
    
    friend vector3D operator^(const vector3D& lhs, const vector3D& rhs) {
        return vector3D(lhs[1]*rhs[2] - lhs[2]*rhs[1], lhs[2]*rhs[0] - lhs[0]*rhs[2], lhs[0]*rhs[1] - lhs[1]*rhs[0]);
    }
    
    double getLength() const { return std::sqrt(*this * *this); }
    double getAngle(const vector3D& oth) const {
        return std::acos(*this * oth / (this->getLength() * oth.getLength()));
    }
    bool equalLength(const vector3D& oth) const {
        return (*this - oth).getLength() < 1e-12;
    }
    
    friend bool operator==(const vector3D& lhs, const vector3D& rhs) { return lhs._crdVec == rhs._crdVec; }
    friend std::ostream& operator<<(std::ostream& os, const vector3D& vec) {
        std::for_each(vec._crdVec.cbegin(), vec._crdVec.cend(), [&](double x) { os << x << ' '; });
        return os;
    }
    
    ~vector3D() {}
private:
    std::array<double, 3> _crdVec;
};

#endif
