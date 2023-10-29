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
}
