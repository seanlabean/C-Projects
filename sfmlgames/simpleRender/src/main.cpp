#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
    sf::RenderWindow    m_window;
    bool                m_running = true;
    sf::CircleShape     circ(50.);
    circ.setPosition(100., 100.);

    std::cout << "test";
    std::cout << "about to create window";
    m_window.create(sf::VideoMode(980, 720), "Assignment 2");
    std::cout << "created window";
    m_window.setFramerateLimit(30);
    while (m_running)
    {
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            if (event.type == sf::Event::KeyPressed)
            {
                std::cout << "Key pressed with code = " << event.key.code << "\n";
            }
        }
        std::cout << "start render ";
        m_window.clear();
        //m_window.draw(circ);
        m_window.display();
        std::cout << "done with Render";;
    }

}
