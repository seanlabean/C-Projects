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
    std::string name, fontLoc="fonts/MiraiSeu.ttf", circleLabel="circ", rectLabel="rect";
    int fontSize, fontColorR, fontColorG, fontColorB;
    int circleColorR, circleColorG, circleColorB;
    int rectColorR, rectColorG, rectColorB;
    int wWidth, wHeight; 
    float circleX, circleY, rectX, rectY;
    float circleVx, circleVy, rectVx, rectVy;
    float circleR, rectW, rectH;
    float winBuff = 50;
    sf::Font myFont;

    std::vector<sf::CircleShape> circles;
    std::vector<sf::RectangleShape> rectangles;
    std::vector<float> rectanglesVx, rectanglesVy, circlesVx, circlesVy;
    std::vector<sf::Text> rectanglesLabel, circlesLabel;

    while (fin >> name)
    {
        if (name=="Window")
        {
            fin >> wWidth >> wHeight;
        } else if (name=="Font")
        {
            fin >> fontLoc >> fontSize >> fontColorR >> fontColorG >> fontColorB;
            // start Font instance, load Font and check for success
            if(!myFont.loadFromFile(fontLoc))
            {
                std::cerr << "Could not load font!\n";
                exit(-1);
            }

        } else if (name=="Circle")
        {
            fin >> circleLabel >> circleX >> circleY >> circleVx >> circleVy >> circleColorR >> circleColorG >> circleColorB >> circleR;

            sf::CircleShape c(circleR);
            c.setFillColor(sf::Color(circleColorR,circleColorG,circleColorB));
            c.setPosition(circleX, circleY);

            circles.push_back(c);
            circlesVx.push_back(circleVx);
            circlesVy.push_back(circleVy);
            sf::Text text(circleLabel, myFont, fontSize);
            circlesLabel.push_back(text);
        }else if (name=="Rectangle")
        {
            fin >> rectLabel >> rectX >> rectY >> rectVx >> rectVy >> rectColorR >> rectColorG >> rectColorB >> rectW >> rectH;

            sf::Vector2f rSize(rectW, rectH);
            sf::RectangleShape rect(rSize);
            rect.setPosition(rectX, rectY);
            rect.setFillColor(sf::Color(rectColorR,rectColorG,rectColorB));

            rectangles.push_back(rect);
            rectanglesVx.push_back(rectVx);
            rectanglesVy.push_back(rectVy);
            sf::Text text(rectLabel, myFont, fontSize);
            rectanglesLabel.push_back(text);
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

    sf::RenderWindow window(sf::VideoMode(wWidth, wHeight), "Bouncing Balls");
    window.setFramerateLimit(60);

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

        sf::Vector2f bgSize(window.getSize().x, window.getSize().y);
        sf::RectangleShape background(bgSize);
        //background.setPosition(0, 0);
        background.setFillColor(sf::Color(91,79,255));
        window.draw(background);

        for (int ind = 0; ind < circles.size(); ++ind)
        {
            auto& circ = circles[ind];
            auto& cVx = circlesVx[ind];
            auto& cVy = circlesVy[ind];
            auto& cLabel = circlesLabel[ind];

            circ.setPosition(circ.getPosition().x + cVx, circ.getPosition().y + cVy);
            cLabel.setPosition(circ.getPosition().x, circ.getPosition().y);
            window.draw(circ);
            window.draw(cLabel);

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
            auto& rLabel = rectanglesLabel[ind];
            
            rect.setPosition(rect.getPosition().x + rVx, rect.getPosition().y + rVy);
            rLabel.setPosition(rect.getPosition().x, rect.getPosition().y);
            window.draw(rect);
            window.draw(rLabel);

            if (rect.getPosition().x < 0 - winBuff || rect.getPosition().x + rect.getSize().x > wWidth + winBuff)
            {
                rVx = -1.0*rVx;
            } else if (rect.getPosition().y < 0 - winBuff || rect.getPosition().y + rect.getSize().y*0.975 > wHeight + winBuff)
            {
                rVy = -1.0*rVy;
            }
        }
        // set and place title
        sf::Text title("Welcome, traveler.", myFont, fontSize);
        title.setPosition(winBuff, wHeight-(float)title.getCharacterSize()-winBuff);

        window.draw(title);
        window.display();       // controls OpenGL display buffers
    }

    return 0;
}