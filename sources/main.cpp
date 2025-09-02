#include "raylib.h"

#include "geometry.h"

#include "triangular_mesh.h"

#include <vector>

#define SCREEN_WIDTH (1280)
#define SCREEN_HEIGHT (720)

#define WINDOW_TITLE "MESH_GEN"

/*
First time build:
mkdir build
cd build
cmake ..
cmake --build .
./Debug/mesh_gen.exe

After that:
cmake --build .
./Debug/mesh_gen.exe
*/

int main()
{
    float spacing = 50;
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);

    std::vector<go::Node> polygon_nodes;
    std::vector<go::Node> glob_nodes;
    std::vector<go::Triangle> triangles;

    bool mesh_created = false;

    while (!WindowShouldClose())
    {
        BeginDrawing();
         ClearBackground(BLACK);

        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            if(mesh_created){
                polygon_nodes.clear();
                triangles.clear();
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
            
            msh::triangulate_mesh(polygon, spacing, triangles, glob_nodes);
            
            polygon_nodes.clear();
            mesh_created = true;

            std::vector<to_fem::Triangle_ref> ref_triangles = to_fem::convert_to_fem(glob_nodes, triangles);
            to_fem::print_mesh(glob_nodes, ref_triangles);
        }

        
        for(auto &it:polygon_nodes){
            it.draw();
        }

        for(auto &it:glob_nodes){
            it.draw();
        }
    
        for(auto &element:triangles){
            element.draw();
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
