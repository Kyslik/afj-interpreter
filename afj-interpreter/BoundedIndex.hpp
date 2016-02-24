//
//  BoundedIndex.hpp
//  afj-interpreter
//
//  Created by Martin Kiesel on 23/02/16.
//  Copyright © 2016 Martin Kiesel. All rights reserved.
//

#ifndef BoundedIndex_hpp
#define BoundedIndex_hpp

#include <stdio.h>
#include <limits>
#include <iostream>

class BoundedIndex
{
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

    int curr() const { return current; }

    bool isNextIncrementOverflowing()
    {
        if (current == max) return true;
        return false;
    }

    bool isNextDecrementUnderflowing()
    {
        if (current == min) return true;
        return false;
    }

    bool isNextCrementumFlowing()
    {
        return isNextIncrementOverflowing() || isNextDecrementUnderflowing();
    }

    void debug()
    {
        std::cout << "BoundedIndex Class debug(): " << std::endl;
        std::cout << "\tcurrent: " << current << std::endl;
        std::cout << "\tmax (upper bound): " << max << std::endl;
        std::cout << "\tmin (lower bound): " << min << std::endl;
    }

    void operator++(int)
    {
        if (current == max)
            current = min;
        else
            current++;
    }

    void operator--(int)
    {
        if (current == min)
            current = max;
        else
            current--;
    }

    void operator=(int _current)
    {
        current = _current;
    }
};


#endif /* BoundedIndex_hpp */
