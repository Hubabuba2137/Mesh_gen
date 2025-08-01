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

namespace go{
    struct Triangle{
        Node points[3];
        Segment edges[3];

        Triangle(Node a, Node b, Node c);
        Triangle();

        void draw();
        void draw(float scale);
    };
}