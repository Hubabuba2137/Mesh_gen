#pragma once

#include <raylib.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>
#include <limits>

#include "geometry.h"

struct Graph{
    std::vector<std::vector<int>> adj_mat;
    std::vector<int> connection_list;
    Graph(int n){
        adj_mat= std::vector<std::vector<int> >(n, std::vector<int>(n, 0));
        connection_list.resize(n);
    }

    void print();
    void print_clist();
};
std::vector<int> count_neighbours(std::vector<go::Triangle> triangles);
Graph build_mat(std::vector<go::Triangle> triangles);

std::vector<go::Node> add_boundary_nodes_on_edge(go::Segment seg, int N);
std::vector<go::Node> add_boundary_nodes_on_vertex(go::Vertex shape, float spacing);
void write_node_ids(std::vector<go::Node> &nodes);
void remove_duplicate_nodes(std::vector<go::Node> &nodes);

bool is_node_inside_trian(go::Vertex vert, go::Node node);
float cross(const Vector2& a, const Vector2& b, const Vector2& c);
bool is_convex(const Vector2& prev, const Vector2& curr, const Vector2& next);
bool is_ear(const std::vector<go::Node>& poly, int i);
std::vector<go::Vertex> ear_cut_triangulation(const go::Vertex& polygon);


bool is_node_same(go::Node n1, go::Node n2);
bool have_same_side(go::Vertex v1, go::Vertex v2);
bool have_same_side(go::Triangle v1, go::Triangle v2);
std::vector<go::Vertex> make_quads(std::vector<go::Vertex> &init_triangles);
std::vector<go::Vertex> make_quads(std::vector<go::Triangle> &init_triangles);
std::vector<go::Node> creating_nodes(go::Vertex polygon, float spacing);

go::Triangle super_trian(std::vector<go::Node> &node_list);
bool inside_circumcircle(go::Triangle &triangle, go::Node &point);
bool same_edge(const go::Segment& e1, const go::Segment& e2);
bool is_boundary_edge(const go::Segment& edge, const std::vector<go::Triangle>& bad_triangles);
bool same_triangle(go::Triangle tr1, go::Triangle tr2);
void filter_triangles(std::vector<go::Triangle> &triangles, go::Vertex &polygon);
std::vector<go::Triangle> bowyer_watson(std::vector<go::Node> &node_list);

std::vector<go::Vertex> create_mesh(go::Vertex polygon, float spacing);