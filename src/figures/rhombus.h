#ifndef RHOMBUS_H
#define RHOMBUS_H

#include <iostream>
#include "figure.h"

class Rhombus : public Figure {
public:
//    Rhombus() = default;
    Rhombus() : Figure(), _lenDiags{0, 0} {}
    explicit Rhombus(const std::pair<int, int>& leftVtx, int d1, int d2) : Figure(leftVtx), _lenDiags{d1, d2} {}
    explicit Rhombus(int x, int y, int d1, int d2) : Figure(x, y), _lenDiags{d1, d2} {}
    
//    Rhombus(const Rhombus& oth) = default;
    Rhombus(const Rhombus& oth) : Figure(oth), _lenDiags(oth._lenDiags) {}
    
    // no resource managment (std::pair and no-move-type int) - no move
    
//    Rhombus& operator=(Rhombus oth) = default;
    Rhombus& operator=(const Rhombus& oth);
    
    double getArea() const override { return 0.5*std::get<0>(_lenDiags)*std::get<1>(_lenDiags); }
    std::pair<double, double> getMassCenter() const override {
        return { std::get<0>(_topLeft) + std::get<0>(_lenDiags), //проблема с ориентацией ромба в прос-ве
    }
    
    friend std::istream& operator>>(std::istream& is, Rhombus& fig);
    friend std::ostream& operator<<(std::ostream& is, const Rhombus& fig);
    
private:
    std::pair<int, int> _lenDiags;
};

#endif
