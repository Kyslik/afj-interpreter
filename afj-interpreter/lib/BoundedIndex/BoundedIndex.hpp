//
//  BoundedIndex.hpp
//  afj-interpreter
//
//  Created by Martin Kiesel on 23/02/16.
//  Copyright © 2016 Martin Kiesel. All rights reserved.
//

#ifndef BoundedIndex_hpp
#define BoundedIndex_hpp

#include <limits>
#include <iostream>

class BoundedIndex
{
private:
    int current, min, max;
    
public:
    BoundedIndex(int _current, int _min, int _max) : current(_current), min(_min), max(_max) {}
    BoundedIndex(int _min, int _max) : current(0), min(_min), max(_max) {}
    BoundedIndex(int _current) : current(_current)
    {
        min = std::numeric_limits<int>::min();
        max = std::numeric_limits<int>::max();
    }
    BoundedIndex() : BoundedIndex(0) {}

    inline int curr() const { return current; }
    bool isNextIncrementOverflowing();
    bool isNextDecrementUnderflowing();
    bool isNextCrementumFlowing();
    void debug();
    void operator++(int);
    void operator--(int);
    void operator=(int _current);
};
#endif /* BoundedIndex_hpp */