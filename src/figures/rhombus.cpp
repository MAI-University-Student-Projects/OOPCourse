#include "rhombus.h"

Rhombus& Rhombus::operator=(const Rhombus& oth) {
    if(this != &oth) {
        _lenDiags = oth._lenDiags;
        Figure::operator=(oth);
    }
    return *this;
}

std::pair<double, double> Rhombus::getMassCenter() const {
    
}

std::istream& operator>>(std::istream& is, Rhombus& fig) {
    
}

std::ostream& operator<<(std::ostream& is, const Rhombus& fig) {
    
}
