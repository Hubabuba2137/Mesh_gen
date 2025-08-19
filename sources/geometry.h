#pragma once

#include <raylib.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>
#include <limits>

#include "node.h"
#include "segment.h"
#include "vertex.h"
#include "triangle.h"


namespace go{
    
    inline bool operator==(const Node& n1, const Node& n2) {
        return (n1.pos.x == n2.pos.x && n1.pos.y == n2.pos.y);
    }
    inline bool operator!=(const Node& n1, const Node& n2) {
        return (n1.pos.x != n2.pos.x && n1.pos.y != n2.pos.y);
    }
    
    inline bool operator==(const Triangle& t1, const Triangle& t2) {
        for (int i = 0; i < 3; ++i) {
            bool found = false;
            for (int j = 0; j < 3; ++j) {
                if(t1.points[i] == t2.points[j]) {
                    found = true;
                    break;
                }
            }
            if (!found) return false;
        }
        return true;
    }
}