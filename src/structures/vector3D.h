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
//    vector3D() = default;
    vector3D() : vector3D(0.0, 0.0, 0.0) { }
    explicit vector3D(double x, double y, double z) : _crdVec{x, y, z} {}
//    vector3D(const vector3D&) = default;
    // This is the preferred method to copy raw arrays in C++ and works with all types that can be copied
    // memcopy(_crdVec, oth._crdVec, 3*sizeof(double)) // C-style raw copying
    vector3D(const vector3D& oth) {
        std::copy(oth._crdVec, oth._crdVec + 3, _crdVec);
    }
    
    /*
     A std::array contains its elements embedded directly inside it (not on the heap, referred to by a pointer, like a std::vector does) so you can't move its contents anywhere if std::array doesn't operate resources
     no resources (sthing to be managed like heap allocs to be deleted, files to be closed e.t.c) - no move semantics
     std::array<_T, _Num> can be moved (and has move constructor itself) if _T - is movable type (like string); int, double, other primitives and some types are not movable
     */
    
    vector3D& operator=(const vector3D& oth); // .cpp
//    vector3D& operator=(const vector3D& oth) = default;
    
    double& operator[](const size_t i);
    const double& operator[](const size_t i) const;
    vector3D& operator+=(const vector3D& oth);
    vector3D& operator-=(const vector3D& oth);
    vector3D& operator*=(double cf);
    
    friend vector3D operator+(const vector3D& lhs, const vector3D& rhs);
    friend vector3D operator-(const vector3D& lhs, const vector3D& rhs);
    friend vector3D operator*(double lhs, const vector3D& rhs);
    friend double operator*(const vector3D& lhs, const vector3D& rhs);
    
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
    
    friend bool operator==(const vector3D& lhs, const vector3D& rhs);
    friend std::ostream& operator<<(std::ostream& os, const vector3D& vec);
    
    ~vector3D() = default;
private:
    double _crdVec[3];
};

#endif

/* implementation in header := implicitly inline method
 with '#include' directive compiler copies its resources to other files so
 inline means that in different transition units method-code will be copied inplace & not executed by CALL (actually there is a special 'inline' keyword)

 we exchange run-time with compile-time so in some cases it could be optimization of runtime (ex. one-line methods)
 conclusion: slower compiling, need more memory usage, but faster running

 other usage - templates: compiler generates different type definitions for template class in compile-time depending on template-class usage in code */


