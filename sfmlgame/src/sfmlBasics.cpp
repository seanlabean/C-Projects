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
    float width, height, radius;
    int ColorR, ColorG, ColorB;
};

int main()
{
    std::string filename="config";
    std::ifstream fin(filename);
    std::string name, fontLoc, circleColor, rectColor;
    int fontSize, fontColorR, fontColorG, fontColorB;
    int circleColorR, circleColorG, circleColorB;
    int rectColorR, rectColorG, rectColorB;
    int wWidth, wHeight; 
    float circleX, circleY, rectX, rectY;
    float circleVx, circleVy, rectVx, rectVy;
    float circleR, rectW, rectH;

    std::vector<sf::CircleShape> circles;
    std::vector<sf::RectangleShape> rectangles;

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
            sf::CircleShape c(circleR);
            c.setFillColor(sf::Color(circleColorR,circleColorG,circleColorB));
            c.setPosition(circleX, circleY);
            
            circles.push_back(c);

        }else if (name=="Rectangle")
        {
            fin >> rectColor >> rectX >> rectY >> rectVx >> rectVy >> rectColorR >> rectColorG >> rectColorB >> rectW >> rectH;
            sf::Vector2f rSize(rectW, rectH);
            sf::RectangleShape rect(rSize);
            rect.setPosition(rectX, rectY);
            rect.setFillColor(sf::Color(rectColorR,rectColorG,rectColorB));

            rectangles.push_back(rect);

        }else
        {
            std::cerr << "Line head is not recognized, skipping.";
        }
    }

    //readFromConfig("config");

    // create window of size w*h pixels
    // top left of window is (0,0) and bottom right is (w,h)

    sf::RenderWindow window(sf::VideoMode(wWidth, wHeight), "SFML works!");
    window.setFramerateLimit(60);

    // make a shape, set its size, color, position, velocity
    //sf::CircleShape circle(circleR);
    //circle.setFillColor(sf::Color(circleColorR,circleColorG,circleColorB));
    //circle.setPosition(circleX, circleY);

    //sf::Vector2f rSize(rectW, rectH);
    //sf::RectangleShape rect(rSize);
    //rect.setPosition(rectX, rectY);
    //rect.setFillColor(sf::Color(rectColorR,rectColorG,rectColorB));
    //rect.setOutlineColor(sf::Color::Red);
    //rect.setOutlineThickness(15);

    // start Font instance, load Font and check for success
    sf::Font myFont;

    if(!myFont.loadFromFile(fontLoc))
    {
        std::cerr << "Could not load font!\n";
        exit(-1);
    }

    // place text
    sf::Text text("SAMPLE TEXT sample text", myFont, fontSize);
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

        window.clear();
        for (auto& circle : circles)
        {
            window.draw(circle);
            circle.setPosition(circle.getPosition().x + circleVx, circle.getPosition().y + circleVy);
        }
        for (auto& rectangle : rectangles)
        {
            window.draw(rectangle);
            rectangle.setPosition(rectangle.getPosition().x + rectVx, rectangle.getPosition().y + rectVy);
        }
        //circle.setPosition(circle.getPosition().x + circleVx, circle.getPosition().y + circleVy);

        //rect.rotate(0.1);


        //window.draw(circle);
        //window.draw(rect);
        window.draw(text);
        window.display();       // controls OpenGL display buffers
    }

    return 0;
}