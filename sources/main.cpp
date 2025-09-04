#include "raylib.h"

#include "geometry.h"

#include "mesh.h"

#include <vector>

#define SCREEN_WIDTH (1280)
#define SCREEN_HEIGHT (720)

#define WINDOW_TITLE "MESH_GEN"

int main()
{
    float spacing = 100;
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);

    std::vector<go::Node> polygon_nodes;
    std::vector<go::Node> glob_nodes;
    std::vector<go::Vertex> mesh;

    bool mesh_created = false;

    while (!WindowShouldClose())
    {
        BeginDrawing();
         ClearBackground(BLACK);

        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            if(mesh_created){
                polygon_nodes.clear();
                mesh.clear();
                glob_nodes.clear();
                mesh_created = false;
            }

            float x = GetMousePosition().x;
            float y = GetMousePosition().y;
            go::Node temp(x,y);
            polygon_nodes.push_back(temp);
        }

        if(IsKeyPressed(KEY_ENTER)){
            go::Vertex polygon(polygon_nodes);
            
            mesh = msh::create_quad_mesh(polygon, spacing, glob_nodes);
            
            polygon_nodes.clear();
            mesh_created = true;

            std::vector<to_fem::Quad_ref> ref_quads = to_fem::convert_to_fem(glob_nodes, mesh);
            to_fem::print_mesh(glob_nodes, ref_quads);
        }

        
        for(auto &it:polygon_nodes){
            it.draw();
        }

        for(auto &it:glob_nodes){
            it.draw();
        }
    
        for(auto &element:mesh){
            element.draw();
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
