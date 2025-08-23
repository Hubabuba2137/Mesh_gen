#include <raylib.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>
#include <limits>
#include <unordered_set>

#include "geometry.h"
#include "meshing.h"

std::vector<go::Node> add_boundary_nodes_on_edge(go::Segment seg, int N){
    go::Node A = seg.tab[0];
    go::Node B = seg.tab[1];

    auto x_u = [&](float u) { return A.pos.x + (B.pos.x - A.pos.x) * u; };
    auto y_u = [&](float u) { return A.pos.y + (B.pos.y - A.pos.y) * u; };

    std::vector<float> u(N+2);
    u[0] = 0;
    for(float i =1; i<=N; i++){
        u[i] = i/(float)N;
    }

    std::vector<go::Node> nodes;
    for(float it:u){
        go::Node temp(x_u(it),y_u(it));

        nodes.emplace_back(temp);
    }

    return nodes;
}

std::vector<go::Node> add_boundary_nodes_on_vertex(go::Vertex shape, float spacing){
    std::vector<go::Node> nodes;
    float L=0;
    for(auto it:shape.edges){
        L+=it.len();
    }

    int N_total = (int)(L/spacing);
    int N = N_total/shape.edges.size();

    for(auto edge:shape.edges){

        std::vector<go::Node> temp_nodes = add_boundary_nodes_on_edge(edge, (int)N);

        for(auto it:temp_nodes){
            nodes.emplace_back(it);
        }
    }

    remove_duplicate_nodes(nodes);
    
    write_node_ids(nodes);

    return nodes;
}

void write_node_ids(std::vector<go::Node> &nodes){
    for(int i=0; i<nodes.size(); i++){
        nodes[i].id = i;
    }
}

struct NodeHash {
    std::size_t operator()(const std::pair<float, float>& pos) const {
        return std::hash<float>()(pos.first) ^ (std::hash<float>()(pos.second) << 1);
    }
};

void remove_duplicate_nodes(std::vector<go::Node> &nodes) {
    std::unordered_set<std::pair<float, float>, NodeHash> unique_positions;
    auto it = nodes.begin();
    while (it != nodes.end()) {
        auto pos = std::make_pair(it->pos.x, it->pos.y);
        if (unique_positions.find(pos) != unique_positions.end()) {
            it = nodes.erase(it);
        } else {
            unique_positions.insert(pos);
            ++it;
        }
    }
}

bool is_node_inside_trian(go::Vertex vert, go::Node node) {
    go::Node a = vert.vertices[0];
    go::Node b = vert.vertices[1];
    go::Node c = vert.vertices[2];
    
    Vector2 p = node.pos;
    
    float cross1 = (b.pos.x - a.pos.x) * (p.y - a.pos.y) - (b.pos.y - a.pos.y) * (p.x - a.pos.x);
    float cross2 = (c.pos.x - b.pos.x) * (p.y - b.pos.y) - (c.pos.y - b.pos.y) * (p.x - b.pos.x);
    float cross3 = (a.pos.x - c.pos.x) * (p.y - c.pos.y) - (a.pos.y - c.pos.y) * (p.x - c.pos.x);
    
    bool has_same_sign = (cross1 >= 0 && cross2 >= 0 && cross3 >= 0) || (cross1 <= 0 && cross2 <= 0 && cross3 <= 0);
    return has_same_sign;
}

float cross(const Vector2& a, const Vector2& b, const Vector2& c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

bool is_convex(const Vector2& prev, const Vector2& curr, const Vector2& next) {
    return cross(prev, curr, next) > 0; 
}


bool is_ear(const std::vector<go::Node>& poly, int i) {
    int prev = (i - 1 + poly.size()) % poly.size();
    int next = (i + 1) % poly.size();

    if (!is_convex(poly[prev].pos, poly[i].pos, poly[next].pos)){
        return false;
    }

    go::Vertex triangle({ poly[prev], poly[i], poly[next] });

    for (int j = 0; j < poly.size(); ++j) {
        if (j == prev || j == i || j == next){
            continue;
        }
        if (is_node_inside_trian(triangle, poly[j])){
            return false;
        }
    }

    return true;
}

std::vector<go::Vertex> ear_cut_triangulation(go::Vertex& polygon) {
    std::vector<go::Node> verts = polygon.vertices;
    std::vector<go::Vertex> triangles;

    if(verts.size()<3){
        std::cerr<<"Polygon too small to triangulate\n";
    }

    while (verts.size() > 3) {
        bool ear_found = false;

        for (size_t i = 0; i < verts.size(); ++i) {
            if (is_ear(verts, i)) {
                int prev = (i - 1 + verts.size()) % verts.size();
                int next = (i + 1) % verts.size();

                std::vector<go::Node> tri = { verts[prev], verts[i], verts[next] };
                triangles.push_back(go::Vertex(tri));
                verts.erase(verts.begin() + i);
                ear_found = true;
                break;
            }
        }

        if (!ear_found) {
            break;
        }
    }

    if (verts.size() == 3) {
        triangles.push_back(go::Vertex(verts));
    }

    return triangles;
}

bool is_node_same(go::Node n1, go::Node n2){
    float tolerance = 0.1;

    if(std::abs(n1.pos.x - n2.pos.x) < tolerance && std::abs(n1.pos.y - n2.pos.y) < tolerance){
        return true;
    }
    else{
        return false;
    }
}

bool have_same_side(go::Vertex v1, go::Vertex v2){

    for(auto&it:v1.edges){
        for(auto&that:v2.edges){
            if((is_node_same(it.tab[0], that.tab[0]) && is_node_same(it.tab[1], that.tab[1])) ||
                (is_node_same(it.tab[0], that.tab[1]) && is_node_same(it.tab[1], that.tab[0]))) {
                return true;
            }
        }
    }

    return false;
}

bool have_same_side(go::Triangle v1, go::Triangle v2){

    for(auto&it:v1.edges){
        for(auto&that:v2.edges){
            if((is_node_same(it.tab[0], that.tab[0]) && is_node_same(it.tab[1], that.tab[1])) ||
                (is_node_same(it.tab[0], that.tab[1]) && is_node_same(it.tab[1], that.tab[0]))) {
                return true;
            }
        }
    }

    return false;
}

//---------------------------------------------------------------------
//graph structure o represent triangles connections
void Graph::print()
{
    int n = adj_mat.size();
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                std::cout << adj_mat[i][j] << " ";
            }
            std::cout << "\n";
        }
}

void Graph::print_clist()
{
    for(int i=0; i<connection_list.size(); i++){
        std::cout<<i<<": "<<connection_list[i]<<"\n";
    }
}

std::vector<int> count_neighbours(std::vector<go::Triangle> triangles) {
    std::vector<int> count(triangles.size(), 0);

    for (size_t i = 0; i < triangles.size(); i++) {
        int n = 0;

        for (size_t j = 0; j < triangles.size(); j++) {
            if (i != j && have_same_side(triangles[i], triangles[j])) {
                n++;
                if (n == 3) {
                    break;
                }
            }
        }

        count[i] = n;
    }

    return count;
}

Graph build_mat(std::vector<go::Triangle> triangles)
{
    Graph graph(triangles.size());

    for (size_t i = 0; i < triangles.size(); i++) {
        int n = 0;

        for (size_t j = 0; j < triangles.size(); j++) {
            if (i != j && have_same_side(triangles[i], triangles[j])) {
                n++;
                graph.adj_mat[i][j] = 1;
                if (n == 3) {
                    break;
                }
            }
        }

        graph.connection_list[i] = n;
    }

    return graph;
}



//---------------------------------------------------------------

std::vector<go::Vertex> make_quads(std::vector<go::Triangle> &init_triangles){
    std::vector<go::Triangle> triangles = init_triangles;
    std::vector<go::Vertex> quads;

    int max_iter = 100;
    int iter = 0;

    if(triangles.size()%2==0){
        //łączenie trójkątów w czworokąty
        while(!triangles.empty() && iter < max_iter){
            iter++;
            go::Triangle tr1 = triangles[0];
            go::Triangle tr2 = triangles[1];
    
            if(have_same_side(tr1, tr2)){
                std::vector<go::Node> temp_nodes;
                for(auto&node_1: tr1.points){
                    temp_nodes.push_back(node_1);
                }
                for(auto&node_2: tr2.points){
                    temp_nodes.push_back(node_2);
                }
    
                remove_duplicate_nodes(temp_nodes);
    
                if(temp_nodes.size() == 4){
                    go::Vertex temp_quad(temp_nodes);
                    quads.push_back(temp_quad);
    
                    triangles.erase(triangles.begin());
                    triangles.erase(triangles.begin());
                }
            }
        }
    }
    else{
        for(auto&triangle:triangles){
            go::Node n1 = triangle.points[0];
            go::Node n2 = triangle.points[1];
            go::Node n3 = triangle.points[2];
    
            go::Node center((n1.pos.x+ n2.pos.x+n3.pos.x)/3,(n1.pos.y+ n2.pos.y+n3.pos.y)/3);
    
            go::Node n4((n1.pos.x+n2.pos.x)/2,(n1.pos.y+n2.pos.y)/2);
            go::Node n5((n2.pos.x+n3.pos.x)/2,(n2.pos.y+n3.pos.y)/2);
            go::Node n6((n3.pos.x+n1.pos.x)/2,(n3.pos.y+n1.pos.y)/2);
    
            std::vector<go::Node> vert1_ns = {n1, n4, center, n6};
            std::vector<go::Node> vert2_ns = {n4, n2, n5, center};
            std::vector<go::Node> vert3_ns = {n5, n3, n6, center};
    
            go::Vertex vert1(vert1_ns);
            go::Vertex vert2(vert2_ns);
            go::Vertex vert3(vert3_ns);
    
            quads.push_back(vert1);
            quads.push_back(vert2);
            quads.push_back(vert3);
        }
    }


    return quads;
}

std::vector<go::Vertex> make_quads(std::vector<go::Vertex> &init_triangles){
    std::vector<go::Vertex> triangles = init_triangles;
    std::vector<go::Vertex> quads;

    int max_iter = 100;
    int iter = 0;

    if(triangles.size()%2==0){
        //łączenie trójkątów w czworokąty
        while(!triangles.empty() && iter < max_iter){
            iter++;
            go::Vertex tr1 = triangles[0];
            go::Vertex tr2 = triangles[1];
    
            if(have_same_side(tr1, tr2)){
                std::vector<go::Node> temp_nodes;
                for(auto&node_1: tr1.vertices){
                    temp_nodes.push_back(node_1);
                }
                for(auto&node_2: tr2.vertices){
                    temp_nodes.push_back(node_2);
                }
    
                remove_duplicate_nodes(temp_nodes);
    
                if(temp_nodes.size() == 4){
                    go::Vertex temp_quad(temp_nodes);
                    quads.push_back(temp_quad);
    
                    triangles.erase(triangles.begin());
                    triangles.erase(triangles.begin());
                }
            }
        }
    }
    else{
        for(auto&triangle:triangles){
            go::Node n1 = triangle.vertices[0];
            go::Node n2 = triangle.vertices[1];
            go::Node n3 = triangle.vertices[2];
    
            go::Node center((n1.pos.x+ n2.pos.x+n3.pos.x)/3,(n1.pos.y+ n2.pos.y+n3.pos.y)/3);
    
            go::Node n4((n1.pos.x+n2.pos.x)/2,(n1.pos.y+n2.pos.y)/2);
            go::Node n5((n2.pos.x+n3.pos.x)/2,(n2.pos.y+n3.pos.y)/2);
            go::Node n6((n3.pos.x+n1.pos.x)/2,(n3.pos.y+n1.pos.y)/2);
    
            std::vector<go::Node> vert1_ns = {n1, n4, center, n6};
            std::vector<go::Node> vert2_ns = {n4, n2, n5, center};
            std::vector<go::Node> vert3_ns = {n5, n3, n6, center};
    
            go::Vertex vert1(vert1_ns);
            go::Vertex vert2(vert2_ns);
            go::Vertex vert3(vert3_ns);
    
            quads.push_back(vert1);
            quads.push_back(vert2);
            quads.push_back(vert3);
        }
    }


    return quads;
}

std::vector<go::Node> creating_nodes(go::Vertex polygon, float spacing){
    std::vector<go::Vertex> triangles = ear_cut_triangulation(polygon);
    
    std::vector<go::Vertex> quads = make_quads(triangles);
    
    float L=0;
    for(auto&edge:polygon.edges){
        L+=edge.len();
    }

    int N = (int)L/spacing;

    std::vector<go::Node> result;
    for(auto &quad: quads){
        //creating internal points
        std::vector<go::Node> edge_0_nodes = add_boundary_nodes_on_edge(quad.edges[0], N);
        std::vector<go::Node> edge_2_nodes = add_boundary_nodes_on_edge(quad.edges[2], N);

        std::vector<go::Node> temp_nodes;
        
        for(size_t i = 0; i < edge_0_nodes.size(); i++) {
            remove_duplicate_nodes(edge_0_nodes);
            remove_duplicate_nodes(edge_2_nodes);
            
            go::Node A = edge_0_nodes[i];
            go::Node B = edge_2_nodes[edge_0_nodes.size()-1-i];

            go::Segment seg(A, B);
        
            temp_nodes = add_boundary_nodes_on_edge(seg, N);
            
            for(auto&node:temp_nodes){
                result.push_back(node);
            }
        }
    }
    return result;
}

go::Triangle super_trian(std::vector<go::Node> &node_list){
    float max_x, max_y, min_x, min_y;
    
    max_x = node_list[0].pos.x;
    max_y = node_list[0].pos.y;
    min_x = node_list[0].pos.x;
    min_y = node_list[0].pos.y;
    for(auto&it:node_list){
        if(it.pos.x > max_x){
            max_x = it.pos.x;
        }

        if(it.pos.x < min_x){
            min_x = it.pos.x;
        }

        if(it.pos.y > max_y){
            max_y = it.pos.y;
        }

        if(it.pos.y < min_y){
            min_y = it.pos.y;
        }
    }

    float w = max_x - min_x;
    float h = max_y - min_y;

    go::Node p1(min_x - w*0.1f, min_y-h);
    go::Node p2(min_x+w*1.7f, min_y+h*0.5f);
    go::Node p3(min_x-w*0.1f, min_y+h*2.0f);
    
    go::Triangle super_triangle(p1, p2, p3);
    return super_triangle;
}

bool inside_circumcircle(go::Triangle &triangle, go::Node &point){
    Vector2 A = triangle.points[0].pos;
    Vector2 B = triangle.points[1].pos;
    Vector2 C = triangle.points[2].pos;
    Vector2 D = point.pos;
    
    double adx = A.x - D.x;
    double ady = A.y - D.y;
    double bdx = B.x - D.x;
    double bdy = B.y - D.y;
    double cdx = C.x - D.x;
    double cdy = C.y - D.y;
    
    double bcdet = bdx * cdy - bdy * cdx;
    double cadet = cdx * ady - cdy * adx;
    double abdet = adx * bdy - ady * bdx;
    
    double alift = adx * adx + ady * ady;
    double blift = bdx * bdx + bdy * bdy;
    double clift = cdx * cdx + cdy * cdy;
    
    double det = alift * bcdet + blift * cadet + clift * abdet;
    
    double area_ABC = (B.x - A.x) * (C.y - A.y) - (C.x - A.x) * (B.y - A.y);
    
    if (area_ABC == 0.0) {
        return false;
    }
    
    return (det * area_ABC > 0);
}
bool same_triangle(go::Triangle tr1, go::Triangle tr2) {
    int matches = 0;
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (is_node_same(tr1.points[i], tr2.points[j])) {
                matches++;
                break;
            }
        }
    }
    
    return matches == 3;
}

inline bool same_edge(const go::Segment& e1, const go::Segment& e2) {
    return (is_node_same(e1.tab[0], e2.tab[0]) && is_node_same(e1.tab[1], e2.tab[1])) ||
           (is_node_same(e1.tab[0], e2.tab[1]) && is_node_same(e1.tab[1], e2.tab[0]));
}

bool is_boundary_edge(const go::Segment& edge, const std::vector<go::Triangle>& bad_triangles) {
    int count = 0;
    for (const auto& triangle : bad_triangles) {
        for (const auto& tri_edge : triangle.edges) {
            if (same_edge(edge, tri_edge)) {
                count++;
                if (count > 1) return false;
            }
        }
    }
    return count == 1;
}

void filter_triangles(std::vector<go::Triangle> &triangles, go::Vertex &polygon){
    std::vector<go::Triangle> temp_triangles;
    temp_triangles.reserve(triangles.size());

    for(auto &tr:triangles){
        float cent_x = (tr.points[0].pos.x + tr.points[1].pos.x + tr.points[2].pos.x)/3;
        float cent_y = (tr.points[0].pos.y + tr.points[1].pos.y + tr.points[2].pos.y)/3;

        go::Node ccentroid(cent_x, cent_y);
        if(polygon.is_node_inside(ccentroid)){
            temp_triangles.push_back(tr);
        }
    }

    triangles = temp_triangles;
}

std::vector<go::Triangle> bowyer_watson(std::vector<go::Node>& node_list) {
    if (node_list.empty()) {
        return {};
    }

    std::vector<go::Triangle> triangulation;
    triangulation.reserve(2 * node_list.size());

    go::Triangle super = super_trian(node_list);
    triangulation.push_back(super);

    std::vector<go::Triangle> bad_triangles;
    std::vector<go::Segment> polygon;
    bad_triangles.reserve(triangulation.capacity() / 4);
    polygon.reserve(32);

    for (go::Node& node : node_list) {
        bad_triangles.clear();
        
        for (go::Triangle& triangle : triangulation) {
            if (inside_circumcircle(triangle, node)) {
                bad_triangles.push_back(triangle);
            }
        }
        
        if (bad_triangles.empty()) {
            continue;
        }

        polygon.clear();
        for (const go::Triangle& bad_tr : bad_triangles) {
            for (const go::Segment& edge : bad_tr.edges) {
                if (is_boundary_edge(edge, bad_triangles)) {
                    polygon.push_back(edge);
                }
            }
        }

        auto new_end = std::remove_if(triangulation.begin(), triangulation.end(),
            [&bad_triangles](const go::Triangle& tr) {
                for (const go::Triangle& bad_tr : bad_triangles) {
                    if (same_triangle(tr, bad_tr)) {
                        return true;
                    }
                }
                return false;
            });

        triangulation.erase(new_end, triangulation.end());

        triangulation.reserve(triangulation.size() + polygon.size());
        for (const go::Segment& edge : polygon) {
            triangulation.emplace_back(edge.tab[0], edge.tab[1], node);
        }
    }

    std::vector<go::Triangle> final_triangulation;
    final_triangulation.reserve(triangulation.size());
    
    for (const go::Triangle& tr : triangulation) {
        bool shares_super_vertex = false;
        
        for (int i = 0; i < 3 && !shares_super_vertex; i++) {
            for (int j = 0; j < 3; j++) {
                if (is_node_same(tr.points[i], super.points[j])) {
                    shares_super_vertex = true;
                    break;
                }
            }
        }
        
        if (!shares_super_vertex) {
            final_triangulation.push_back(tr);
        }
    }

    return final_triangulation;
}

std::vector<go::Vertex> create_mesh(go::Vertex polygon, float spacing){
    //interpoalting nodes
    std::vector<go::Node> nodes = creating_nodes(polygon, spacing);

    //creating triangle mesh with delonay triangulation
    std::vector<go::Triangle> triangles = bowyer_watson(nodes);

    //filtering triangles outside polygon
    filter_triangles(triangles, polygon);

    //connecting triangles into quads
    std::vector<go::Vertex> mesh = make_quads(triangles);

    return mesh;
}