#ifndef TRAPEZOID_H
#define TRAPEZOID_H

#include <iostream>
#include "figure.h"

class Trapezoid : public Figure {
public:
//    Trapezoid() = default;
    Trapezoid() : Figure(), _lenWidths{0, 0}, _height{0} {}
    
    explicit Trapezoid(const std::pair<int, int>& leftVtx, const std::pair<int, int>& widths, int h) : Figure(leftVtx), _lenWidths{widths}, _height{h} {}
//    Trapezoid(const Trapezoid& oth) = default;
    Trapezoid(const Trapezoid& oth) : Figure(oth), _lenWidths{oth._lenWidths}, _height{oth._height} {}
    
//    Trapezoid& operator=(const Trapezoid& oth) = default; //same with assignments
    Trapezoid& operator=(const Trapezoid& oth);
    
    // no resource managment (std::pair and no-move-type int) - no move
    
    double getArea() const override { return 0.5 * (std::get<0>(_lenWidths) + std::get<1>(_lenWidths)) * _height; }
    std::pair<double, double> getMassCenter() const override;
    
    friend std::istream& operator>>(std::istream& is, Trapezoid& fig);
    friend std::ostream& operator<<(std::ostream& is, const Trapezoid& fig);
    
private:
    std::pair<int, int> _lenWidths;
    int _height;
};

#endif
