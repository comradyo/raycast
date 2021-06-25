#include <SFML/Graphics.hpp>
using namespace sf;

#include <cmath>

const float Pi = 3.14159f;
const float dAngle = 0.05f;
const float dStep = 0.1f;

struct Vector2D
{
    Vector2D(const float &_x, const float &_y);
    Vector2D& operator=(const Vector2D& other);
    void rotateToAngle(const float &angle);
    Vector2D operator+(const Vector2D &other);
    Vector2D operator-(const Vector2D &other);
    Vector2D &operator+=(const Vector2D &other);
    Vector2D &operator-=(const Vector2D &other);
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
    Camera(Vector2D pos, Vector2D dir);
    Vector2D position;
    Vector2D direction;
    void rotateLeft();
    void rotateRight();
    void moveForward();
    void moveBackward();
    Angle angle;
private:

};