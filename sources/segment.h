#pragma once

#include <raylib.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>
#include <limits>

#include "node.h"

namespace go{
    struct Segment{
        //segement is made up of two points
        Node tab[2];
        
        Segment(Node node_start, Node node_end);
        Segment();
        void draw();
        void draw(float scale);
        void move(Vector2);
        
        float len();
            
        bool operator<(const Segment &other) const {
        if (tab[0].pos.x != other.tab[0].pos.x)
            return tab[0].pos.x < other.tab[0].pos.x;
        if (tab[0].pos.y != other.tab[0].pos.y)
            return tab[0].pos.y < other.tab[0].pos.y;
        if (tab[1].pos.x != other.tab[1].pos.x)
            return tab[1].pos.x < other.tab[1].pos.x;
            return tab[1].pos.y < other.tab[1].pos.y;
        }
    };
}