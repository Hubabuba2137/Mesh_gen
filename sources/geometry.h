#pragma once

#include <raylib.h>
#include <vector>

namespace go{
    struct Node{
        //Node is a single point representem by a small circle
        int id;
        Vector2 pos;
        float radius = 3.0;
        Color node_color = GREEN;

        Node(float x_in, float y_in);
        Node(Vector2 pos_in);
        Node(Vector2 pos_in, int id);
        Node();
        void draw();
        void draw(float scale);
        void draw(Color color);

        void move(Vector2 vec);
        void change_color(Color new_color);
    };

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

    struct Triangle{
        Node points[3];
        Segment edges[3];

        Triangle(Node a, Node b, Node c);
        Triangle();

        void draw();
        void draw(float scale);
    };

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

        bool is_node_inside(const Node &node);
        void sort_vertices_by_position();

        bool ray_intersects_segment(Node point, Segment seg);
    };
    
    inline bool operator==(const Node& n1, const Node& n2) {
        return (n1.pos.x == n2.pos.x && n1.pos.y == n2.pos.y);
    }
    inline bool operator!=(const Node& n1, const Node& n2) {
        return !(n1 == n2);
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