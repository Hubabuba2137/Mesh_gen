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

namespace go{
    struct Vertex{
        //vertex is just a shape made of multiple points (nodes)
        std::vector<Node> vertices;
        std::vector<Segment> edges;
    
        Vertex(std::vector<Node> nodes);
        Vertex();

        void create_edges();
        void draw();
        void draw_nodes();
        void add_vertex(Node node);

        void create_edges(std::vector<Node> nodes);

        bool is_node_inside(Node &node);
        void sort_vertices_by_position();

        bool ray_intersects_segment(Node point, Segment seg);
    };
}