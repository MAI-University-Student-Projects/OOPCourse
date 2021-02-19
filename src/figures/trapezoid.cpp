#include "trapezoid.h"

Trapezoid& Trapezoid::operator=(const Trapezoid& oth) {
    if(this != &oth) {
        _lenWidths = oth._lenWidths;
        _height = oth._height;
        Figure::operator=(oth);
    }
    return *this;
}

std::pair<int, int> Trapezoid::getMassCenter() const {
    
}

std::istream& operator>>(std::istream& is, Trapezoid& fig) {
    
}

std::ostream& operator<<(std::ostream& is, const Trapezoid& fig) {
    
}
