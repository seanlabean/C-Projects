#include <iostream>

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