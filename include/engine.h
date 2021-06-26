#include <SFML/Graphics.hpp>
using namespace sf;

#include <cmath>

struct Vector2D
{
    Vector2D(const float &_x, const float &_y);
    Vector2D &operator=(const Vector2D &other);
    void rotateToAngle(const float &angle);
    Vector2D operator-();
    friend const Vector2D operator+(const Vector2D &first, const Vector2D &second);
    friend const Vector2D operator-(const Vector2D &first, const Vector2D &second);
    Vector2D operator*(const float &value);
    Vector2D operator/(const float &value);
    friend const Vector2D operator*(const Vector2D &first, const float &value);
    friend const Vector2D operator/(const Vector2D &first, const float &value);
    Vector2D &operator+=(const Vector2D &other);
    Vector2D &operator-=(const Vector2D &other);
    float length() const;
    float x;
    float y;
};

struct Angle
{
    Angle(float _value);
    Angle &operator+=(const float &dValue);
    Angle &operator-=(const float &dValue);
    operator float() const;

private:
    float value;
};

struct Camera
{
    Camera(Vector2D pos, Vector2D dir, Vector2D pl);
    Vector2D position;
    Vector2D direction;
    Vector2D plane;
    void rotateLeft();
    void rotateRight();
    void moveForward();
    void moveBackward();
    Angle angle;

private:
};