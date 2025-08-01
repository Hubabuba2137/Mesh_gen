#pragma once

#include <raylib.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>
#include <limits>

#include "segment.h"
#include "node.h"
#include "vertex.h"

namespace go{
    Vertex::Vertex(std::vector<Node> nodes) {
        float centroidX = 0.0f, centroidY = 0.0f;
        for (const auto& node : nodes) {
            centroidX += node.pos.x;
            centroidY += node.pos.y;
        }
        centroidX /= nodes.size();
        centroidY /= nodes.size();

        std::sort(nodes.begin(), nodes.end(), [centroidX, centroidY](const Node& a, const Node& b) {
            float angleA = atan2(a.pos.y - centroidY, a.pos.x - centroidX);
            float angleB = atan2(b.pos.y - centroidY, b.pos.x - centroidX);
            return angleA < angleB;
        });

        vertices = nodes;

        for(int i=0; i<this->vertices.size(); i++){
            vertices[i].id = i;
        }

        create_edges();
    }

    Vertex::Vertex() {
        vertices = {};
        edges = {};
    }

    void Vertex::sort_vertices_by_position() {
        std::sort(vertices.begin(), vertices.end(), [](const Node& a, const Node& b) {
            if (a.pos.x != b.pos.x) {
                return a.pos.x < b.pos.x;
            }
            return a.pos.y < b.pos.y;
        });
        create_edges();
    }
    
    void Vertex::create_edges(){
        std::vector<Segment> temp;
        std::vector<Node> vert = this->vertices;
        
        for(size_t i=0; i<vertices.size();i++){
    
            if(i<vertices.size()-1){
                Segment temp_line(vert[i], vert[i+1]);
                temp.push_back(temp_line);
            }
            else{
                Segment temp_line(vert[i], vert[0]);
                temp.push_back(temp_line);
            }
        }

        this->edges = temp;
    }

    void Vertex::create_edges(std::vector<Node> nodes){
        std::vector<Segment> temp;
        
        for(size_t i=0; i<nodes.size();i++){
            temp.emplace_back(go::Segment(nodes[i], nodes[(i+1)%nodes.size()]));
        }

        this->vertices = nodes;
        this->edges = temp;
    }
    void Vertex::draw_nodes(){
        for(size_t i =0; i<vertices.size();i++){
            vertices[i].draw();
        }
    }
    
    void Vertex::draw(){
        for(size_t i =0; i<edges.size();i++){
            edges[i].draw();
        }
    
        for(size_t i =0; i<vertices.size();i++){
            vertices[i].draw();
        }
    }
    
    void Vertex::add_vertex(Node node){
        vertices.push_back(node);
        //sort_vertices_by_position();
        create_edges();
    }

    bool Vertex::ray_intersects_segment(Node point, Segment seg) {
        float x = point.pos.x, y = point.pos.y;
        float x1 = seg.tab[0].pos.x, y1 = seg.tab[0].pos.y;
        float x2 = seg.tab[1].pos.x, y2 = seg.tab[1].pos.y;
    
        if ((y1 > y) != (y2 > y)) {
            float intersectX = x1 + (y - y1) * (x2 - x1) / (y2 - y1);
            return x < intersectX;
        }
        return false;
    }

    bool Vertex::is_node_inside(Node &node) {
        
        int intersections = 0;
        for (const Segment &seg : this->edges) {
            if (ray_intersects_segment(node, seg)) {
                ++intersections;
            }
        }
        if(intersections%2 == 0){
            return false;
        }
        else{
            return true;
        }
        
    }
}