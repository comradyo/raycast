#include "engine.h"
#include "configurations.cpp"
//==============================================================
//Vector2D
//==============================================================

Vector2D::Vector2D(const float &_x, const float &_y) : x(_x), y(_y) {}

Vector2D &Vector2D::operator=(const Vector2D &other)
{
    this->x = other.x;
    this->y = other.y;
    return *this;
}

void Vector2D::rotateToAngle(const float &angle)
{
    float _x = x;
    float _y = y;
    x = _x * cos(angle) - _y * sin(angle);
    y = _x * sin(angle) + _y * cos(angle);
}

Vector2D Vector2D::operator+(const Vector2D &other)
{
    return Vector2D(this->x + other.x, this->y + other.y);
}

Vector2D Vector2D::operator-(const Vector2D &other)
{
    return Vector2D(this->x - other.x, this->y - other.y);
}

Vector2D &Vector2D::operator+=(const Vector2D &other)
{
    this->x += other.x;
    this->y += other.y;
    return *this;
}

Vector2D &Vector2D::operator-=(const Vector2D &other)
{
    this->x -= other.x;
    this->y -= other.y;
    return *this;
}

//==============================================================
//Angle
//==============================================================

Angle::Angle(float _value = 0.f) : value(_value) {}

Angle &Angle::operator+=(const float &dValue)
{
    value += dValue;
    while (value > 2 * Pi)
    {
        value -= 2 * Pi;
    }
    while (value < 0)
    {
        value += 2 * Pi;
    }
    return *this;
}

Angle &Angle::operator-=(const float &dValue)
{
    value -= dValue;
    while (value > 2 * Pi)
    {
        value -= 2 * Pi;
    }
    while (value < 0)
    {
        value += 2 * Pi;
    }
    return *this;
}

Angle::operator float() const
{
    return this->value;
}

//==============================================================
//Camera
//==============================================================

Camera::Camera(Vector2D pos, Vector2D dir, Vector2D pl) : position(pos), direction(dir), plane(pl) {}

//Возможно, все знаки придётся поменять
void Camera::rotateLeft()
{
    angle -= dAngle;
    direction.rotateToAngle(-dAngle);
    plane.rotateToAngle(-dAngle);
}

//Возможно, все знаки придётся поменять
void Camera::rotateRight()
{
    angle += dAngle;
    direction.rotateToAngle(dAngle);
    plane.rotateToAngle(dAngle);
}

//Возможно, придётся поменять sin на cos и наоборот
void Camera::moveForward()
{
    position.x += dStep * cos(angle);
    position.y += dStep * sin(angle);
}

//Возможно, придётся поменять sin на cos и наоборот
void Camera::moveBackward()
{
    position.x -= dStep * cos(angle);
    position.y -= dStep * sin(angle);
}