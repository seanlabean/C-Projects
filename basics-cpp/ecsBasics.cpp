#include <iostream>
#include <vector>
#include <map>

// An Entity is just a container for components
//
// Entity Manager
//   handles the creation, storage, and lifetime of 
//   ALL Entities.
//   EntityManager functions:
//      addEntity, removeEntity, storage, bookkeeping
//   Called Factory design pattern. 
class Entity
{
    // private vars
    const size_t      m_id    = 0; // unique id for each entity
    const std::string m_tag   = "Default"; // tag for entity groups
    bool              m_alive = true;
// public vars
public:
    std::shared_ptr<CTransform> cTransform;
    std::shared_ptr<CName>      cName;
    std::shared_ptr<CShape>     cShape;
    std::shared_ptr<CBBox>      cBBox;
    // constructor
    Entity(const std::string& tag, size_t id);
};

// define obbject shorthands. We will be using vectors of 
// pointers to entities. Also, creating separate vector
// maps for entities with specific tags
typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::map   <std::string, EntityVec>  EntityMap;
class EntityManager
{
    EntityVec m_entities
    EntityMap m_entityMap;
    size_t    m_totalEntities = 0;
public:
    EntityManager();
    std::shared_ptr<Entity> addEntity(const std:string& tag);
    EntityVec& getEntities();
    EntityVec& getEntities(const std::string& tag);
};


// Why are writing our own 2D vector class
// (SFML already has Vector2f)? We can add other 
// great functionality, and it's good to learn!
class Vec2
{
public:
    float x = 0;
    float y = 0;

    Vec2() {}

    Vec2(float xin, float yin)
        : x(xin)
        , y(yin)
    {
    }
    // adding operator (create new vector)
    Vec2 operator + (const Vec2& rhs) const
    {
        return Vec2(x + rhs.x, y + rhs.y);
    }
    // '+=' operator
    void operator += (const Vec2& rhs)
    {
        x += rhs.x;
        y += rhs.y;
    }
    // add operator (modifies one vector)
    // method chaining returns reference to itself
    Vec2& add(const Vec2& v)
    {
        x += v.x;
        y += v.y;
        return *this;
    }
    // What is chaining good for?
    Vec2& scale(float s)
    {
        x *=s;
        y *=s;
        return *this;
    }
    Vec2& rotate(float deg)
    {
        return *this;
    }

    // Other good tools. Distance formula.
    float dist(const Vec2& v) const
    {
        return sqrtf((v.x-x)*(v.x-x)+(v.y-y)*(v.y-y));
    }
};

int main(int argc, char * argv[])
{
    Vec2 v1(100, 200);
    Vec2 v2(50, 75);

    // using Vec2 class '+' operator
    Vec2 v3 = v1 + v2;
    std::cout << v3.x << " " << v3.y;

    // another way to add
    v1.add(v2);

    v1 += v2;

    Vec2 v3 (1000,2000);

    // How to add v2 and v3 to v1?
    // use "chained" add function
    v1.add(v2).add(v3);

    // Put all these chained functions together!
    float s = 5.0;
    v1.scale(s).rotate(30).add(v2);
    // Can also call one at a time
    v1.scale(s);
    v1.rotate(30);

    float dist = v1.dist(v2);
}