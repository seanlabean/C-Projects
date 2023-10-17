#include <iostream>

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
};

int main(int argc, char * argv[])
{
    Vec2 v1(100, 200);
    Vec2 v2(50, 75);

    Vec2 v3(v1.x + v2.x, v1.y + v2.y);

    std::cout << v3.x << " " << v3.y;
}