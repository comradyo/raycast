#include <iostream>
using namespace std;

#include <SFML/Graphics.hpp>
using namespace sf;

#include <cmath>

const float Pi = 3.14159f;
const float dAngle = 0.05f;

struct Angle
{
    Angle(float _value = 0.f) : value(_value){};
    Angle &operator+=(const float &dValue)
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
    Angle &operator-=(const float &dValue)
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
    operator float() const
    {
        return this->value;
    }

private:
    float value;
};

struct Camera
{
    Camera(Vector2f pos, Vector2f dir) : position(pos), direction(dir){};
    Vector2f position;
    Vector2f direction;
    void rotateLeft()
    {
        angle += dAngle;
        this->rotate();
    }
    void rotateRight()
    {
        angle -= dAngle;
        this->rotate();
    }

private:
    Angle angle;
    void rotate()
    {
        direction.x = direction.x * cos(angle) - direction.y * sin(angle);
        direction.y = direction.x * sin(angle) + direction.y * cos(angle);
    }
};

const int windowWidth = 400;
const int windowHeight = 300;

const int worldWidth = 10;
const int worldHeight = 10;

char world[worldWidth][worldHeight] =
    {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

int rectWidth = 10;
int rectHeight = 10;

int main()
{
    RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "SFML works!");
    Keyboard keyboard;

    int posX = 4;
    int posY = 4;

    int dirX = 5;
    int dirY = 4;

    RectangleShape rect;
    rect.setSize(Vector2f(rectWidth, rectHeight));

    Camera camera(Vector2f(posX, posY), Vector2f(dirX, dirY));

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case (Event::KeyPressed):
                cout << camera.direction.x << " " << camera.direction.y << endl;
                switch (event.key.code)
                {
                case (Keyboard::A):
                    camera.rotateLeft();
                    break;
                case (Keyboard::D):
                    camera.rotateRight();
                    break;
                default:
                    break;
                }
                break;
            case (Event::Closed):
                window.close();
                break;
            default:
                break;
            }
        }

        window.clear();

        for (int i = 0; i < worldWidth; ++i)
        {
            for (int j = 0; j < worldHeight; ++j)
            {
                rect.setPosition(Vector2f(i * rectWidth, j * rectHeight));
                switch (world[i][j])
                {
                case 1:
                    rect.setFillColor(Color::Cyan);
                    break;
                default:
                    rect.setFillColor(Color::Magenta);
                    break;
                }
                window.draw(rect);
            }
        }

        rect.setFillColor(Color::Green);
        rect.setPosition(Vector2f(camera.direction.x * rectWidth, camera.direction.y * rectHeight));
        window.draw(rect);

        window.display();
    }
}