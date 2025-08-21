#include "raylib.h"

#include "geometry.h"
#include "meshing.h"
#include "triangle.h"
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
    std::vector<go::Node> nodes;

    std::vector<go::Triangle> triangles;

    bool mesh_created = false;

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(DARK_GRAY);

        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            if(mesh_created){
                polygon_nodes.clear();
                nodes.clear();
                triangles.clear();
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
            nodes = creating_nodes(polygon, spacing);
            triangles = bowyer_watson(nodes);


            polygon_nodes.clear();
        }


        for(auto&it:polygon_nodes){
            it.draw();
        }
        
        for(auto&it:nodes){
            it.draw();
        }

        for(auto&it:triangles){
            it.draw();
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
