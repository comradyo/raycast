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

Vector2D Vector2D::operator-()
{
    return Vector2D(-this->x, -this->y);
}

const Vector2D operator+(const Vector2D &first, const Vector2D &second)
{
    return Vector2D(first.x + second.x, first.y + second.y);
}

const Vector2D operator-(const Vector2D &first, const Vector2D &second)
{
    return Vector2D(first.x - second.x, first.y - second.y);
}

Vector2D Vector2D::operator*(const float &value)
{
    return Vector2D(this->x * value, this->y * value);
}

Vector2D Vector2D::operator/(const float &value)
{
    return Vector2D(this->x / value, this->y / value);
}

const Vector2D operator*(const Vector2D &vec2D, const float &value)
{
    return Vector2D(vec2D.x * value, vec2D.y * value);
}

const Vector2D operator/(const Vector2D &vec2D, const float &value)
{
    return Vector2D(vec2D.x / value, vec2D.y / value);
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

float Vector2D::length() const
{
    return sqrt(this->x * this->x + this->y * this->y);
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

//===============================
//Functions
//===============================
char world[worldWidth][worldHeight];
const float deltaScreenZ = 1.f / windowHeight;

void setWorld()
{
    for (int i = 0; i < worldWidth; ++i)
    {
        for (int j = 0; j < worldHeight; ++j)
        {
            if (i == 0 || i == worldWidth - 1 || j == 0 || j == worldHeight - 1)
            {
                world[i][j] = 1;
            }
            else
            {
                world[i][j] = 0;
            }
        }
    }
    world[2][2] = 1;
    world[2][3] = 1;
    world[3][2] = 1;
}

void drawWals(Camera &camera, RenderWindow &window)
{
    Vertex line[2];

    for (int i = 0; i < windowWidth; ++i)
    {
        //cameraX - коэффициент вектора плоскости экрана, находится в диапозоне от [1, -1]
        float cameraX = -(2 * i / double(windowWidth) - 1);
        //Направление луча
        Vector2D rayDir = camera.direction + camera.plane * cameraX;

        int mapX = int(camera.position.x);
        int mapY = int(camera.position.y);

        //Расстояния до первой встречной координаты X и Y соответственно
        float sideDistX;
        float sideDistY;

        //Расстояния, которые луч проходит, чтобы перейти к следующим координатам X и Y соответственно
        //(По сути, должно быть abs(rayDir.length()/rayDir.x)), но нам эта длина не важна
        float deltaDistX = (rayDir.y == 0) ? 0 : ((rayDir.x == 0) ? 1 : abs(1 / rayDir.x));
        float deltaDistY = (rayDir.x == 0) ? 0 : ((rayDir.y == 0) ? 1 : abs(1 / rayDir.y));
        float perpWallDist;

        int stepX;
        int stepY;

        bool hit = false;
        int side;

        if (rayDir.x < 0)
        {
            stepX = -1;
            sideDistX = (camera.position.x - mapX) * deltaDistX;
        }
        else
        {
            stepX = 1;
            sideDistX = (mapX + 1.0 - camera.position.x) * deltaDistX;
        }
        if (rayDir.y < 0)
        {
            stepY = -1;
            sideDistY = (camera.position.y - mapY) * deltaDistY;
        }
        else
        {
            stepY = 1;
            sideDistY = (mapY + 1.0 - camera.position.y) * deltaDistY;
        }

        while (!hit)
        {
            if (sideDistX < sideDistY)
            {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            }
            else
            {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }
            if (world[mapX][mapY] > 0)
            {
                hit = true;
            }
        }

        //Нужно для устранения эффекта рыбьего глаза, хз, как работает
        if (side == 0)
        {
            perpWallDist = (mapX - camera.position.x + (1 - stepX) / 2) / rayDir.x;
        }
        else
        {
            perpWallDist = (mapY - camera.position.y + (1 - stepY) / 2) / rayDir.y;
        }

        int lineHeight = (int)(windowHeight / perpWallDist);

        int drawStart = -lineHeight / 2 + windowHeight / 2;
        if (drawStart < 0)
        {
            drawStart = 0;
        }
        int drawEnd = lineHeight / 2 + windowHeight / 2;
        if (drawStart > windowHeight)
        {
            drawEnd = windowHeight - 1;
        }

        line[0].position = Vector2f(i, drawStart);
        line[0].color = Color::Cyan;
        line[1].position = Vector2f(i, drawEnd);
        line[1].color = Color::Magenta;

        window.draw(line, 2, sf::Lines);
    }
}

void floorCast(Camera &camera, RenderWindow &window, RectangleShape &rect)
{
    RectangleShape line;
    for (int i = 0; i < windowWidth; ++i)
    {
        float cameraX = -(2 * i / double(windowWidth) - 1);
        Vector2D rayDir = camera.direction + camera.plane * cameraX;
        Vector2D dRay = rayDir / floorcastQuality;
        Vector2D ray = camera.position + rayDir;

        float screenZ = 0.0f;
        for (int j = windowHeight; j > windowHeight / 2 + 10; --j)
        {
            //Здесь "+1" - это высота над землей
            float maxLength = (screenZ + 10) * rayDir.length() / (camera.z - screenZ);
            while ((ray - camera.position).length() < maxLength)
            {
                //Здесь должна быть функция, добавляющая dRay в соответствии с кривизной пространства в точке (u, v) по направлению (du, dv)
                ray += curvedVector(ray, dRay);
            }
            if (int(ray.x) % 10 == 0 || int(ray.y) % 10 == 0)
            {
                rect.setFillColor(Color::Cyan);
            }
            else
            {
                rect.setFillColor(Color::Black);
            }

            rect.setPosition(i, j);
            window.draw(rect);
            screenZ += deltaScreenZ;
        }
    }
}

float R = 1000.f;

Vector2D functionXY(const float& u, const float& v)
{
    return Vector2D(u, v);
}

Vector2D curvedVector(const Vector2D &currentPosition, const Vector2D &direction)
{
    return functionXY(direction.x, direction.y);
}
