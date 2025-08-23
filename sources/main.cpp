#include "raylib.h"

#include "geometry.h"
#include "meshing.h"

#include <vector>

#define SCREEN_WIDTH (1280)
#define SCREEN_HEIGHT (720)

#define WINDOW_TITLE "MESH_GEN"

#define DARK_GRAY CLITERAL(Color){30,30,30,255}

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
    float spacing = 200;
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);

    std::vector<go::Node> polygon_nodes;
    std::vector<go::Triangle> triangles;
    std::vector<go::Vertex> mesh;

    bool mesh_created = false;

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(DARK_GRAY);

        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            if(mesh_created){
                polygon_nodes.clear();
                mesh.clear();
                mesh_created = false;
            }

            float x = GetMousePosition().x;
            float y = GetMousePosition().y;
            go::Node temp(x,y);
            polygon_nodes.push_back(temp);
        }

        if(IsKeyPressed(KEY_ENTER)){
            mesh_created = true;
            go::Vertex polygon(polygon_nodes);
            std::vector<go::Node> tr_nodes = creating_nodes(polygon, spacing);
            triangles = bowyer_watson(tr_nodes);

            Graph gr = build_mat(triangles);
            gr.print_adjList();
            mesh = create_mesh(polygon, spacing);

            polygon_nodes.clear();
        }


        for(auto&it:polygon_nodes){
            it.draw();
        }
        
        /*
        for(auto&element:mesh){
            element.draw();
        }*/

        for(auto&tr:triangles){
            tr.draw();
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
