#include <SFML/Graphics.hpp>

#include "Game.h"
#include "Vec2.h"
//#include "Vec2.cpp"

#include <iostream>

int main()
{
    std::cout << "test";
    Game g("config.txt");
    
    g.run(); 
    Vec2 v1(100, 100);
    Vec2 v2(300, 500);

    std::cout << v2.x;
}
