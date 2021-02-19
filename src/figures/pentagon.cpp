#include "pentagon.h"

Pentagon& Pentagon::operator=(const Pentagon& oth) {
    if(this != &oth) {
        _lenSide = oth._lenSide;
        Figure::operator=(oth);
    }
    return *this;
}

std::pair<int, int> Pentagon::getMassCenter() const {
    //auto res = std::make_pair(...)
}

std::istream& operator>>(std::istream& is, Pentagon& fig) {
    
}

std::ostream& operator<<(std::ostream& is, const Pentagon& fig) {
    
}
