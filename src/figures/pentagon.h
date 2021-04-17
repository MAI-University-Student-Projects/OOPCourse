#ifndef PENTAGON_H
#define PENTAGON_H

#include <iostream>
#include "figure.h"

class Pentagon : public Figure {
public:
//    Pentagon() = default;
    Pentagon() : Figure(), _lenSide{0} {}
    explicit Pentagon(int x, int y, int val) : Figure(x, y), _lenSide{val} { }
    explicit Pentagon(const std::pair<int, int>& leftVtx, int val) : Figure(leftVtx), _lenSide{val} { }

//    =default also invokes Figure abstract class
//    Pentagon(const Pentagon& oth) = default;
    Pentagon(const Pentagon& oth) : Figure(oth), _lenSide(oth._lenSide) {}
    
    // no resource managment (std::pair and no-move-type int) - no move
    
    Pentagon& operator=(const Pentagon& oth);
    
    double getArea() const override { return 5.0 * std::pow(_lenSide, 2) / (4.0 * std::tan(0.2 * M_PI)); }
    std::pair<double, double> getMassCenter() const override;
    
    friend std::istream& operator>>(std::istream& is, Pentagon& fig);
    friend std::ostream& operator<<(std::ostream& is, const Pentagon& fig);
    
private:
    int _lenSide;
};

#endif

