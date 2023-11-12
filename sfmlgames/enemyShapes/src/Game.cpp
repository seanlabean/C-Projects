#include "Game.h"

#include <iostream>
#include <fstream>


Game::Game(const std::string & config)
{
    init(config);
}

void Game::init(const std::string & config)
{
    // TODO: read in config file here
    //       use premade PlayerConfig, EnemyConfig, BulletConfig variables
    std::ifstream fin(config);
    std::string name;
    // NOT WORKING FOR SOME REASON...
    fin >> name;
    // while (fin >> name)
    // {
    //     std::cout << "made it here? " << std::flush;
    //     if (name=="Player")
    //     {
    //         std::cout << "REALLY" << std::flush;
    //         fin >> m_playerConfig.SR >> m_playerConfig.CR >> m_playerConfig.FR >> 
    //         m_playerConfig.FG >> m_playerConfig.FB >> m_playerConfig.OR >> 
    //         m_playerConfig.OG >> m_playerConfig.OB >> m_playerConfig.OT >> 
    //         m_playerConfig.V >> m_playerConfig.S;
    //     }
    // }
    // Hardcoding config params here.
    // Window 1280 720 60 0
    // Font fonts/NewHeterodoxMono-Book.otf 24 255 255 255
    // Player 32 32 5 5 5 5 255 0 0 4 8
    // Enemy 32 32 3 3 255 255 2 3 8 90 60
    // Bullet 10 10 20 255 255 255 255 255 255 2 20 90

    m_playerConfig.SR = 42; m_playerConfig.CR = 32; m_playerConfig.FR = 5; m_playerConfig.FG = 5;
    m_playerConfig.FB = 5; m_playerConfig.OT = 5; m_playerConfig.OR = 255; m_playerConfig.OG = 0; 
    m_playerConfig.OB = 0; m_playerConfig.V = 4; m_playerConfig.S = 3.;

    m_enemyConfig.SR = 32; m_enemyConfig.CR = 32; m_enemyConfig.OR = 255; m_enemyConfig.OG = 255;
    m_enemyConfig.OB = 255; m_enemyConfig.OT = 3; m_enemyConfig.VMIN = -8; m_enemyConfig.VMAX = 8;
    m_enemyConfig.L = 8; m_enemyConfig.SI = 8; m_enemyConfig.SMIN = 3; m_enemyConfig.SMAX = 8;

    m_bulletConfig.SR = 10; m_bulletConfig.CR = 10; m_bulletConfig.FR = 20; m_bulletConfig.FR = 255;
    m_bulletConfig.FG = 255; m_bulletConfig.FB = 255; m_bulletConfig.OR = 255; m_bulletConfig.OG = 255;
    m_bulletConfig.OB = 255; m_bulletConfig.OT = 2; m_bulletConfig.V = 20; m_bulletConfig.L = 90;
    m_bulletConfig.S = 255;

    // setup default window parameters
    m_window.create(sf::VideoMode(980, 720), "Assignment 2");
    m_window.setFramerateLimit(60);

    spawnPlayer();
}

void Game::run()
{
    while (m_running)
    {
        m_entities.update();
        sf::Event event;
        if (!m_paused)
        {
            sRender();
            sEnemySpawner();
            sMovement();
            sCollision();
            sUserInput();
        }else 
        {
            sRender();
            sUserInput();
        }
        // increment the current frame 
        // may need to be moved when pause implemented
        m_currentFrame++;
    }
}

void Game::setPaused(bool paused)
{
    m_paused = !paused;
}

// respawn the player in the middle of the screen
void Game::spawnPlayer()
{
    // TODO Finish adding aa properties of the player with the correct values from the config

    // We create every entity by calling the EntityManager.addEntity(tag)
    // This returns a std::shared_ptr<Entity>, so we use 'auto; to save typing
    auto entity = m_entities.addEntity("player");

    // Get center of window and add player entity  there
    float mx = m_window.getSize().x / 2.0f;
    float my = m_window.getSize().y / 2.0f;
    entity->cTransform = std::make_shared<CTransform>(Vec2(mx, my), Vec2(0.0f, 0.0f), 0.0);

    entity->cShape = std::make_shared<CShape>(m_playerConfig.SR, m_playerConfig.S, sf::Color(10, 10, 10), sf::Color(255, 0, 0), 4.0f);

    // Add an input component to the player so that we can use inputs
    entity->cInput = std::make_shared<CInput>();

    // Since we want this Entity to be our player, set our Game's player variable to be this Entity
    // This goes slightly against the Entity Manager paradigm, but we use the player so much it's worth it
    m_player = entity;
}

//spawn an enemy at a random position
void Game::spawnEnemy()
{
    // TODO: make sure the enemy is spawned properly with the m_enemyConfig variables
    //       the enemy must be spawned completely within the bounds of the window.
    //       Spawn enemy with with random velocity
    auto entity = m_entities.addEntity("enemy");

    float ex = rand() % m_window.getSize().x; // NOT correct, need to account for shape radius.
    float ey = rand() % m_window.getSize().y;

    double evx = (static_cast<double>(rand()) / RAND_MAX) * (m_enemyConfig.VMAX - m_enemyConfig.VMIN) + m_enemyConfig.VMIN;
    double evy = (static_cast<double>(rand()) / RAND_MAX) * (m_enemyConfig.VMAX - m_enemyConfig.VMIN) + m_enemyConfig.VMIN;

    entity->cTransform = std::make_shared<CTransform>(Vec2(ex, ey), Vec2(evx, evy), 0.0);

    // The entity's shape will have radius 32, 8 sides, dark grey fill and red outline with thickness 4
    entity->cShape = std::make_shared<CShape>(m_enemyConfig.SR, m_enemyConfig.SMAX, sf::Color(0, 0, 0), sf::Color(255, 255, 255), 4.0f);

    // record when the most recent enemy was spawned
    m_lastEnemySpawnTime = m_currentFrame;
}

// spawns the small enemies when a big one (input entity e) explodes
void Game::spawnSmallEnemies(std::shared_ptr<Entity> e)
{
    // TODO: spawn small enemies at the location of the input enemy e

    // when we create the smaller enemy, we have to read the values of the original eney
    // - spawn a number of small enemies equal to the vertices of the original enemy
    // - set each small enemy to the same color as the original, half the size
    // - small enemies are worth double points of the original enemy
}

// spawns a bullet from a given entity to a target location
void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2 & target)
{
    // TODO implement the spawning of a bullet which travels toward target
    //      - bullet speed is given as a scalar speed
    //      - you must set the velocity by using formula in notes

    //example
    auto bullet = m_entities.addEntity("bullet");
    float playerPosX = entity->cTransform->pos.x;
    float playerPosY = entity->cTransform->pos.y;

    Vec2 b_vel_n = Vec2(target.x - playerPosX, target.y - playerPosY) / Vec2(target.x - playerPosX, target.y - playerPosY).length();
    Vec2 b_v = b_vel_n * m_bulletConfig.V;

    bullet->cTransform = std::make_shared<CTransform>(m_player->cTransform->pos, b_v, 0);
    bullet->cTransform->velocity = b_v;
    bullet->cShape = std::make_shared<CShape>(10, 8, sf::Color(100, 100, 100), sf::Color(255, 0, 0), 2);
    //bullet->cLifespan->total = 100;
    
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
    // TODO: implement your own special weapon
}

void Game::sMovement()
{
    m_player->cTransform->velocity = {0.0, 0.0};
    // implement player movement
    if (m_player->cInput->up)
    {
        m_player->cTransform->velocity.y = -5;
    } else if (m_player->cInput->down)
    {
        m_player->cTransform->velocity.y = 5;
    } else if (m_player->cInput->left)
    {
        m_player->cTransform->velocity.x = -5;
    } else if (m_player->cInput->right)
    {
        m_player->cTransform->velocity.x = 5;
    }
    // TODO
    // maybe player-window buffer check, set vel to 0?

    for (auto e : m_entities.getEntities())
    {
        if (e->cShape->circle.getPosition().x > m_window.getSize().x || e->cShape->circle.getPosition().x < 0.0)
        {
            
            if (e != m_player)
            { 
                e->cTransform->velocity.x *= -1.0;
                e->cTransform->pos.x += e->cTransform->velocity.x; 
            } else
            {
                e->cTransform->velocity.x *= -1.0;
                e->cTransform->pos.x += e->cTransform->velocity.x;
                // This is an example of how to port player from one side of screen to other.
                // This is NOT implemented in y direction yet.

                // if (m_player->cShape->circle.getPosition().x > m_window.getSize().x)
                // {
                //     m_player->cTransform->pos.x = e->cTransform->velocity.x;     
                // } else if (m_player->cShape->circle.getPosition().x < 0.0)
                // {
                //     m_player->cTransform->pos.x = m_window.getSize().x - e->cTransform->velocity.x;
                // }
                
            }

        } else if (e->cShape->circle.getPosition().y > m_window.getSize().y || e->cShape->circle.getPosition().y < 0.0)
        {
            
            e->cTransform->velocity.y *= -1.0;
            e->cTransform->pos.y += e->cTransform->velocity.y;

        } else
        {
            e->cTransform->pos.x += e->cTransform->velocity.x ;
            e->cTransform->pos.y += e->cTransform->velocity.y;
        }
    }
}

void Game::sLifespan()
{
    // TODO: implement all lifespan functionality
    //
    // for all entities
    //      if entity has no lifespan component, skip it
    //      if entity has > 0 remaining lifespan, subtract 1
    //      if it has a lifespan and is alive
    //          scale its alpha channel properly
    //      if it has a lifespan and its time is up
    //          destroy the entity
    for (auto e : m_entities.getEntities())
    {
        if (e->cLifespan)
        {
            if (e->cLifespan->remaining > 0)
            {
                e->cLifespan->remaining -= 1;

            } else
            {
                e->destroy();
            }
        }
    }
}

void Game::sCollision()
{
    // TODO: implement all proper collisions between entities
    //       be sure to use the collision radius, NOT the shape radius
    //for (auto b : m_entities.getEntities("bullet"))
    //{
    //    for (auto e : m_entities.getEntities("enemy"))
    //    {
            // decide whether bullet has hit enemy
    //    }
    //}
}

void Game::sEnemySpawner()
{
    if (m_currentFrame - m_lastEnemySpawnTime > 100)
    {
        spawnEnemy();
    }
}

void Game::sRender()
{
    m_window.clear();
    // Text draw doesnt work?
    // sf::Text title("Welcome, traveler.", m_font, 32);
    // m_window.draw(title);

    for (auto e : m_entities.getEntities())
    {
        // set the position of the shape based on the entity's transform->pos
        e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);

        // set the rotation of the shape based on the entity's transform->angle
        e->cTransform->angle += 1.0f;
        e->cShape->circle.setRotation(e->cTransform->angle);

        // draw the entity's sf::CircleShape
        m_window.draw(e->cShape->circle);
    }
    m_window.display();
}

void Game::sUserInput()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        // this event triggers when the window is closed
        if (event.type == sf::Event::Closed)
        {
            m_running = false;
        }
        // this event is triggered when a key is pressed
        if (event.type == sf::Event::KeyPressed)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::Escape:
                  setPaused(m_paused);
                  std::cout << "Game Paused...\n";
                  break;
            case sf::Keyboard::W:
                //std::cout << "W Key Pressed\n";
                m_player->cInput->up = true;
            case sf::Keyboard::S:
                //std::cout << "S Key Pressed\n";
                m_player->cInput->down = true;
            case sf::Keyboard::A:
                //std::cout << "A Key Pressed\n";
                m_player->cInput->left = true;
            case sf::Keyboard::D:
                //std::cout << "D Key Pressed\n";
                m_player->cInput->right = true;
            case sf::Keyboard::Space:
                //std::cout << "SPACE pressed!\n";
            default: break;
            }
        }
        if (event.type == sf::Event::KeyReleased)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::W:
                //std::cout << "W Key Released\n";
                m_player->cInput->up = false;
            case sf::Keyboard::S:
                //std::cout << "S Key Released\n";
                m_player->cInput->down = false;
            case sf::Keyboard::A:
                //std::cout << "A Key Released\n";
                m_player->cInput->left = false;
            case sf::Keyboard::D:
                //std::cout << "D Key Released\n";
                m_player->cInput->right = false;
            default: break;
            }
        }
        
        if (event.type == sf::Event::Closed)
        {
            m_window.close();
        }

        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                //std::cout << "Left mouse button clicked at (" << event.mouseButton.x << ", "<< event.mouseButton.y << ")\n";
                spawnBullet(m_player, Vec2(event.mouseButton.x, event.mouseButton.y));
            }
        }
    }
}