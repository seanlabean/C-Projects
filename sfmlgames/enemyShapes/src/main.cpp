#include <SFML/Graphics.hpp>

#include "Game.h"
#include "Vec2.h"
//#include "Vec2.cpp"

#include <iostream>

int main()
{
    Game g("config.txt");
    g.run(); 
}
