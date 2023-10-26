#include "Game.h"

#include <iostream>
#include <fstream>

Game::Game(const std::string & config)
{
    init(config);
}

void Game::init(const std::string & path)
{
    // TODO: read in config file here
    //       use premade PlayerConfig, EnemyConfig, BulletConfig variables
    // std::ifstream fin(path)
    // fin >> m_playerConfig.SR >> m_playerConfig.CR

    // setup default window parameters
    m_window.create(sf::VideoMode(1280, 720), "Assignment 2");
    m_window.setFramerateLimit(60);

    spawnPlayer();
}

void Game::run()
{
    // TODO: add pause functionality in here
    //       some systems should function while paused (rendering)
    //       some systems shouldn't (movement / input)
    while (m_running)
    {
        m_entities.update();

        if (!m_paused)
        {
            // put some of these here, or else
        }
        sEnemySpawner();
        sMovement();
        sCollision();
        sUserInput();
        sRender();

        // increment the current frame 
        // may need to be moved when pause implemented
        m_currentFrame++;
    }
}

void Game::setPaused(bool paused)
{
    //TODO, just set the variable
}

// respawn the player in the middle of the screen
void Game::spawnPlayer()
{
    // TODO Finish adding aa properties of the player with the correct values from the config

    // We create every entity by calling the EntityManager.addEntity(tag)
    // This returns a std::shared_ptr<Entity>, so we use 'auto; to save typing
    auto entity = m_entities.addEntity("player");

    //Give this entity a Transform so it spawns at (200,200) with velocity (1,1) and angle 0
    entity->cTransform = std::make_shared<CTransform>(Vec2(200.0f, 200.0f), Vec2(1.0f, 1.0f));

    // The entity's shape will have radius 32, 8 sides, dark grey fill and red outline with thickness 4
    entity->cShape = std::make_shared<CShape>(32.0f, 8, sf::Color(10, 10, 10), sf::Color(255, 0, 0), 4.0f);

    // Add an input component to the player so that we can use inputs
    entity->cInput = std::make_shared<CInput>();

    // Since we want this Entity to be our player, set our Game's player variable to be this Entity
    // This goes slightly against the Entity Manager paradigm, but we use the player so much it's worth it
    m_player = entity;
}

//spawn an enemy at a random position
void Game::spawnEnemy()
{

}