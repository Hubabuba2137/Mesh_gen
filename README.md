# Meshgen

Simple triangular mesh generation, needs more polishing

It subdivides triangles by adding (to the vector of nodes creating a mesh) new nodes in place of triangles center, which are then retriangulated using bowyer-watson algorithm. I've also added requirement for the new triangles to have their area bigger then the mean value at the moment. It makes elements roughly equal in size.

# Setup

To build this project you need cmake and raylib

Clone this repository then paste:

    mkdir build

    cd build

    cmake ..

    cmake --build .
    
    ./Debug/mesh_gen.exe