#include "Vec2.h"
#include <math.h>

Vec2::Vec2()
{
    
}

Vec2::Vec2(float xin, float yin)
    : x(xin), y(yin)
{

}

Vec2 Vec2::operator + (const Vec2& rhs) const
{
    return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator - (const Vec2& rhs) const
{
    return Vec2(x - rhs.x, y - rhs.y);
}

Vec2 Vec2::operator / (const float val) const
{
    return Vec2(x/val,y/val);
}

Vec2 Vec2::operator * (const float val) const
{
    return Vec2(x*val, y*val);
}

bool Vec2::operator == (const Vec2& rhs) const
{
    return (x==rhs.x && y==rhs.y);
}

bool Vec2::operator != (const Vec2& rhs) const
{
    return (x!=rhs.x || y!=rhs.y);
}

Vec2 Vec2::operator += (const Vec2& rhs)
{
    x+=rhs.x;
    y+=rhs.y;
    return *this;
}

Vec2 Vec2::operator -= (const Vec2& rhs)
{
    x-=rhs.x;
    y-=rhs.y;
    return *this;
}

Vec2 Vec2::operator *= (const float val)
{
    x*=val;
    y*=val;
    return *this;
}

Vec2 Vec2::operator /= (const float val)
{
    x/=val;
    y/=val;
    return *this;
}

float Vec2::dist(const Vec2& rhs) const
{
    Vec2 D;
    D = *this - rhs;
    return sqrtf(D.x*D.x + D.y*D.y);
}

float Vec2::length() const
{
    return sqrtf(x*x + y*y);
}
