#pragma once

#include <raylib.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>
#include <limits>

#include "node.h"

namespace go{
    Node::Node(float x_in, float y_in){
        //pos = (Vector2){x_in, SCREEN_HEIGHT-y_in}; //normalny układ współrzędnych
        Vector2 pos_vec{x_in, y_in};
        pos = pos_vec;
    }

    Node::Node(Vector2 pos_in){
        this->pos = pos_in;
        this->id=0;
    }

    Node::Node(Vector2 pos_in, int id){
        this->pos = pos_in;
        this->id = id;
    }

    Node::Node(){
        Vector2 pos_vec{0, 0};
        this->pos = pos_vec;
        this->id = 0;
    }

    void Node::draw(){
        DrawCircleV(pos, radius, node_color);
    }

    void Node::draw(float scale){
        DrawCircleV(pos, radius*scale, node_color);
    }

    void Node::draw(Color color){
        DrawCircleV(pos, radius, color);
    }

    void Node::move(Vector2 vec){
        this->pos.x += vec.x;
        this->pos.y += vec.y;
    }

    void Node::change_color(Color new_color){
        this->node_color = new_color;
    }
}