#include "EntityManager.h"

EntityManager::EntityManager()
{

}

void EntityManager::update()
{
    // TODO: add entities from m_entitiesToAdd to the proper location(s)
    //       - add them to the vector of all entities GOOD
    //       - add them to the vector inside the map, with the tag as a key
    
    for (auto e : m_entitiesToAdd)
    {
        m_entities.push_back(e);
    }

    m_entitiesToAdd.clear();

    // remove dead entites from the vector of all entities
    removeDeadEntities(m_entities);

    // remove dead entities from each vector in the entity map
    // C++17 way of iterating through [key,value] pairs in a map
    for (auto& [tag, entityVec] : m_entityMap)
    {
        removeDeadEntities(entityVec);
    }
}

void EntityManager::removeDeadEntities(EntityVec & vec)
{
    // TODO: remove all dead entities from the input vector
    //       this is called by the update() function

    // look at std::remove_if
    //for (auto e : vec)
    //{
     //   if (!e->isActive())
      //  {

      //  }
    //}
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string & tag)
{
    auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));

    m_entitiesToAdd.push_back(entity);

    return entity;
}

const EntityVec & EntityManager::getEntities()
{
    return m_entities;
}

const EntityVec & EntityManager::getEntities(const std::string & tag)
{
    // TODO this is incorrect, return the correct vector from the map
    return m_entityMap[tag];
}