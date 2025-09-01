#pragma once
#include <vector>
#include "geometry.h"

bool is_node_same(const go::Node &n1, const go::Node &n2);
void write_node_ids(std::vector<go::Node> &nodes);
void remove_duplicate_nodes(std::vector<go::Node> &nodes);
std::vector<go::Node> add_boundary_nodes_on_edge(const go::Segment &seg, int N);
std::vector<go::Node> add_boundary_nodes_on_vertex(const go::Vertex shape, float spacing);
go::Triangle super_trian(const std::vector<go::Node> &node_list);
bool inside_circumcircle(const go::Triangle &triangle, const go::Node &point);
bool same_triangle(const go::Triangle tr1, const go::Triangle tr2);
inline bool same_edge(const go::Segment& e1, const go::Segment& e2);
bool is_boundary_edge(const go::Segment& edge, const std::vector<go::Triangle>& bad_triangles);
void filter_triangles(std::vector<go::Triangle> &triangles, go::Vertex &polygon);

std::vector<go::Triangle> bowyer_watson(std::vector<go::Node>& node_list);
void triangulate_mesh(go::Vertex polygon, float spacing, std::vector<go::Triangle> &triangles, std::vector<go::Node> &nodes);
