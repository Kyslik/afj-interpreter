//
//  BoundedIndex.cpp
//  afj-interpreter
//
//  Created by Martin Kiesel on 23/02/16.
//  Copyright © 2016 Martin Kiesel. All rights reserved.
//

#include "BoundedIndex.hpp"

bool BoundedIndex::isNextIncrementOverflowing()
{
    if (current == max) return true;
    return false;
}

bool BoundedIndex::isNextDecrementUnderflowing()
{
    if (current == min) return true;
    return false;
}

bool BoundedIndex::isNextCrementumFlowing()
{
    return isNextIncrementOverflowing() || isNextDecrementUnderflowing();
}


void BoundedIndex::debug()
{
    std::cout << "BoundedIndex Class debug(): " << std::endl;
    std::cout << "\tcurrent: " << current << std::endl;
    std::cout << "\tmax (upper bound): " << max << std::endl;
    std::cout << "\tmin (lower bound): " << min << std::endl;
}

void BoundedIndex::operator++(int)
{
    if (current == max)
        current = min;
    else
        current++;
}

void BoundedIndex::operator--(int)
{
    if (current == min)
        current = max;
    else
        current--;
}

void BoundedIndex::operator=(int _current)
{
    current = _current;
}