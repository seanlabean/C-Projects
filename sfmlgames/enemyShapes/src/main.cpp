#include <SFML/Graphics.hpp>

#include "Game.h"

#include <iostream>

int main()
{
    // Vec2 v1(100, 100);
    // Vec2 v2(200, 300);
    // v1 += v2;
    // std::cout << v1.x << " " << v1.y << "\n" << std::flush; 
    // std::cout << v2.x << " " << v2.y << "\n" << std::flush; 
    // v1*=3;
    // std::cout << v1.x << " " << v1.y << "\n" << std::flush; 
    
    // float d = v1.dist(v2);
    // std::cout << d << "\n" << std::flush; 
    Game g("config.txt");
    g.run(); 
}
