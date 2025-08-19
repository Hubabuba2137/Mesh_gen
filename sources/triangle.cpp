#include <raylib.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>
#include <limits>

#include "segment.h"
#include "node.h"
#include "triangle.h"

namespace go{
    Triangle::Triangle(Node a, Node b, Node c){
        points[0] = a;
        points[1] = b;
        points[2] = c;
    
        edges[0] = Segment(a,b);
        edges[1] = Segment(b,c);
        edges[2] = Segment(a,c);
    }
    
    Triangle::Triangle()
    {
    }
    void Triangle::draw()
    {
        for(Segment it: edges){
            it.draw();
        }
    }
    void Triangle::draw(float scale)
    {
        for(Segment it: edges){
            it.draw(scale);
        }
    }

}
