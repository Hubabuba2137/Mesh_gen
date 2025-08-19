#pragma once

#include <raylib.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>
#include <limits>

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
}