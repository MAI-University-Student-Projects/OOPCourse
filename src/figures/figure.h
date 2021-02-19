#ifndef FIGURE_H
#define FIGURE_H

#include <iostream>
#include <algorithm>
#include <cmath>

//abstract class but not interface
class Figure {
public:
    virtual double getArea() const = 0;
    virtual std::pair<int, int> getMassCenter() const = 0;
    
    friend bool operator<(const Figure& lhs, const Figure& rhs) { return lhs.getArea() < rhs.getArea(); }
    friend bool operator>(const Figure& lhs, const Figure& rhs) { return lhs.getArea() < rhs.getArea(); }
    friend bool operator==(const Figure& lhs, const Figure& rhs) { return lhs.getArea() == rhs.getArea(); }
    
    virtual ~Figure() { };
protected:
//    Figure() = default;
    Figure() : Figure(0, 0) { }
    explicit Figure(int x, int y) : _topLeft{x, y} {}
    explicit Figure(const std::pair<int, int>& leftCoord) : _topLeft(leftCoord) {}
//    Figure(const Figure&) = default;
    Figure(const Figure& oth) : _topLeft(oth._topLeft) {}
    
    // no resource managment (std::pair and no-move-type int) - no move
    
    /* allow assignments only for derived classes */
//    Figure& operator=(const Figure&) = default;
    Figure& operator=(const Figure& oth) {
        if(this != &oth)
            _topLeft = oth._topLeft;
        return *this;
    }
    
    std::pair<int, int> _topLeft;
};

#endif

/* wanted to implement spaceship operator <=> := combined <, <=, >, >=, ==, !=,
 BUT AT THE MOMENT CONSIDERED DANGEROUS FOR WRAPPERS(vectors, arrays, and other stl)
    spaceship operator, returns _Type_ordering(::less -> <0, ::greater -> >0, ::equal -> ==0)
    (=default) -  each member is compared through spaceship operator by compiler (=deleted for std::array and std::vector so overload or write own implementation)

    if comparison (equal-check) of 2 obj-s of same Type -> std::strong_ordering(equality) else -> std::weak_ordering(equality)
    std::partial_ordering - comparison with smthing more irrelevant like NaN-s
 casts: std::strong_ordering -> std::weak_ordering -> std::partial_ordering (no <-)
 so if overload implementation with return as std::strong_ordering contains smwhere std::weak_ordering it causes compiler error
    in templates works with classes with default spaceship op (with default it s certainly gonna convert to general type, where this operator already implemented)
    for std::string spaceship is equvalent to strcmp() */
