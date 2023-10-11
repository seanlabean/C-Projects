#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>

class AShape
{
public:
    sf::CircleShape circle;
    sf::RectangleShape rect;
    sf::Color color;
    std::string name;
    float px, py, vx, vy;
};
void readFromConfig(const std::string& filename)
{
    std::ifstream fin(filename);
    std::string name, fontLoc, circleColor, rectColor;
    int fontSize, fontColorR, fontColorG, fontColorB;
    int circleColorR, circleColorG, circleColorB;
    int rectColorR, rectColorG, rectColorB;
    int wWidth, wHeight; 
    float circleX, circleY, rectX, rectY;
    float circleVx, circleVy, rectVx, rectVy;
    float circleR, rectW, rectH;

    while (fin >> name)
    {
        if (name=="Window")
        {
            fin >> wWidth >> wHeight;
        } else if (name=="Font")
        {
            fin >> fontLoc >> fontSize >> fontColorR >> fontColorG >> fontColorB;
        } else if (name=="Circle")
        {
            fin >> circleColor >> circleX >> circleY >> circleVx >> circleVy >> circleColorR >> circleColorG >> circleColorB >> circleR;
        }else if (name=="Rectangle")
        {
            fin >> rectColor >> rectX >> rectY >> rectVx >> rectVy >> rectColorR >> rectColorG >> rectColorB >> rectW >> rectH;
        }
    }
    std::cout << name << " " << wWidth << " " << wHeight << "\n";
    std::cout << fontLoc << " " << fontSize << "\n";
    std::cout << rectColor << " " << rectW << " " << rectH << "\n";
}
int main()
{
    readFromConfig("config");

    // create window of size w*h pixels
    // top left of window is (0,0) and bottom right is (w,h)
    const int wWidth = 1000;
    const int wHeight  = 600;
    sf::RenderWindow window(sf::VideoMode(wWidth, wHeight), "SFML works!");
    window.setFramerateLimit(60);

    // make a shape, set its size, color, position, velocity
    sf::CircleShape circle(100.f);
    circle.setFillColor(sf::Color::Magenta);
    circle.setPosition(200.f, 200.f);
    float circleMoveSpeed = 0.01f;

    float rx = 300;
    float ry = 200;
    sf::Vector2f rSize(rx, ry);
    sf::RectangleShape rect(rSize);
    rect.setPosition(100.f, 5.f);
    rect.setFillColor(sf::Color(255,255,0));
    rect.setOutlineColor(sf::Color::Red);
    rect.setOutlineThickness(15);

    // start Font instance, load Font and check for success
    sf::Font myFont;

    if(!myFont.loadFromFile("fonts/MiraiSeu.ttf"))
    {
        std::cerr << "Could not load font!\n";
        exit(-1);
    }

    // place text
    sf::Text text("SAMPLE TEXT sample text", myFont, 36);
    text.setPosition(0, wHeight-(float)text.getCharacterSize());

    // reading in multiple shapes

    // main loop
    while (window.isOpen())
    {
        // event loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed)
            {
                // print code of key that was pressed
                std::cout << "Key pressed with code = " << event.key.code << "\n";
                if (event.key.code == sf::Keyboard::X)
                {
                    // reverse the shape velocity direction here.
                }
            }
        }

        float sx = 0.5f;
        float sy = 0.5f;
        circle.setPosition(circle.getPosition().x + sx, circle.getPosition().y + sy);

        rect.rotate(0.1);

        window.clear();
        window.draw(circle);
        window.draw(rect);
        window.draw(text);
        window.display();       // controls OpenGL display buffers
    }

    return 0;
}