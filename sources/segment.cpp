#include <raylib.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>
#include <limits>

#include "segment.h"
#include "node.h"

namespace go{
    Segment::Segment(Node node_start, Node node_end){
        tab[0] = node_start;
        tab[1] = node_end;
    }

    Segment::Segment(){}

    void Segment::draw(){
        DrawLineV(tab[0].pos, tab[1].pos, WHITE);
        tab[0].draw();
        tab[1].draw();
    }

    void Segment::draw(float scale){
        DrawLineV(tab[0].pos, tab[1].pos, WHITE);
        tab[0].draw(scale);
        tab[1].draw(scale);
    }
        
    void Segment::move(Vector2 vec){
        this->tab[0].pos.x+=vec.x;
        this->tab[0].pos.y+=vec.y;
        this->tab[1].pos.x+=vec.x;
        this->tab[1].pos.y+=vec.y;
    }

    float Segment::len(){
        float dx = tab[1].pos.x - tab[0].pos.x;
        float dy = tab[1].pos.y - tab[0].pos.y;
        return sqrt(dx * dx + dy * dy);
    }

}