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
    float winBuff = 50;

    std::vector<sf::CircleShape> circles;
    std::vector<sf::RectangleShape> rectangles;
    std::vector<float> rectanglesVx;
    std::vector<float> rectanglesVy;
    std::vector<float> circlesVx;
    std::vector<float> circlesVy;

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
            circlesVx.push_back(circleVx);
            circlesVy.push_back(circleVy);
        }else if (name=="Rectangle")
        {
            fin >> rectColor >> rectX >> rectY >> rectVx >> rectVy >> rectColorR >> rectColorG >> rectColorB >> rectW >> rectH;

            sf::Vector2f rSize(rectW, rectH);
            sf::RectangleShape rect(rSize);
            rect.setPosition(rectX, rectY);
            rect.setFillColor(sf::Color(rectColorR,rectColorG,rectColorB));

            rectangles.push_back(rect);
            rectanglesVx.push_back(rectVx);
            rectanglesVy.push_back(rectVy);
        }else if (name=="Buffer")
        {
            fin >> winBuff;
        }else
        {
            // Not sure how to only print out first line unit once
            // since this will trigger each time any line unit
            // in the offending line isn't recognized.
            continue;
        }
    }

    // create window of size w*h pixels
    // top left of window is (0,0) and bottom right is (w,h)

    sf::RenderWindow window(sf::VideoMode(wWidth, wHeight), "SFML works!");
    window.setFramerateLimit(60);

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
                //std::cout << "Key pressed with code = " << event.key.code << "\n";
                if (event.key.code == sf::Keyboard::X)
                {
                    // reverse the shape velocity direction here.
                }
            }
        }

        window.clear();

        for (int ind = 0; ind < circles.size(); ++ind)
        {
            auto& circ = circles[ind];
            auto& cVx = circlesVx[ind];
            auto& cVy = circlesVy[ind];

            window.draw(circles[ind]);
            circ.setPosition(circ.getPosition().x + cVx, circ.getPosition().y + cVy);

            if (circ.getPosition().x < 0 - winBuff || circ.getPosition().x + circ.getRadius()*2.0 > wWidth + winBuff)
            {
                cVx = -1.0*cVx;
            } else if (circ.getPosition().y < 0 - winBuff || circ.getPosition().y + circ.getRadius()*2.0 > wHeight + winBuff)
            {
                cVy = -1.0*cVy;
            }
        }
        for (int ind = 0; ind < rectangles.size(); ++ind)
        {
            auto& rect = rectangles[ind];
            auto& rVx = rectanglesVx[ind];
            auto& rVy = rectanglesVy[ind];
            
            window.draw(rect);
            rect.setPosition(rect.getPosition().x + rVx, rect.getPosition().y + rVy);

            if (rect.getPosition().x < 0 - winBuff || rect.getPosition().x + rect.getSize().x > wWidth + winBuff)
            {
                rVx = -1.0*rVx;
            } else if (rect.getPosition().y < 0 - winBuff || rect.getPosition().y + rect.getSize().y*0.975 > wHeight + winBuff)
            {
                rVy = -1.0*rVy;
            }
        }
        window.draw(text);
        window.display();       // controls OpenGL display buffers
    }

    return 0;
}