#include <iostream>
using namespace std;

#include <SFML/Graphics.hpp>
using namespace sf;

#include <cmath>
#include "engine.h"
#include "configurations.cpp"

char world[worldWidth][worldHeight] =
    {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

void drawMap(const Camera &camera, RectangleShape &rect, RenderWindow &window)
{
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

    Vector2f _position(camera.position.x * rectWidth, camera.position.y * rectHeight);
    Vector2f _direction(camera.direction.x * rectWidth, camera.direction.y * rectHeight);
    Vector2f _plane(camera.plane.x * rectWidth, camera.plane.y * rectHeight);

    rect.setFillColor(Color::Green);
    rect.setPosition(_position);
    window.draw(rect);

    rect.setFillColor(Color::Red);
    rect.setPosition(_position + _direction);
    window.draw(rect);

    rect.setFillColor(Color::Blue);
    rect.setPosition(_position + _direction + _plane);
    window.draw(rect);
    rect.setPosition(_position + _direction - _plane);
    window.draw(rect);

    Vector2D deltaPlane = (camera.plane * 2) / windowWidth;
    Vector2D rayStartVector = camera.position + camera.direction + camera.plane;

    rect.setSize(Vector2f(1, 1));
    rect.setFillColor(Color::Red);

    for (int i = 0; i < windowWidth; ++i)
    {
        Vector2D rayVector = rayStartVector;
        Vector2D dRay = (rayStartVector - camera.position) / 10.f;

        while (world[int(rayVector.x)][int(rayVector.y)] == 0 && (rayVector - rayStartVector).length() < 50)
        {
            rect.setFillColor(Color((rayVector - rayStartVector).length() * 100, 0, 225));
            rect.setPosition(rayVector.x * rectWidth, rayVector.y * rectHeight);
            window.draw(rect);
            rayVector += dRay;
        }

        rayStartVector -= deltaPlane;
    }

    rect.setSize(Vector2f(rectWidth, rectHeight));
}

void drawWals(Camera &camera, RenderWindow &window)
{
    //Вектор, который будет прибавляться к текущему вектору плоскости
    Vector2D deltaPlane = (camera.plane * 2) / windowWidth;
    //С какой позиции стартует луч
    Vector2D rayStartVector = camera.position + camera.direction + camera.plane;

    Vertex line[2];

    for (int i = 0; i < windowWidth; ++i)
    {

        Vector2D rayVector = rayStartVector;
        Vector2D dRay = (rayStartVector - camera.position) / 10.f;

        while (world[int(rayVector.x)][int(rayVector.y)] == 0 && (rayVector - rayStartVector).length() < 50)
        {
            rayVector += dRay;
        }

        line[0].position = Vector2f(i, windowHeight / 2 + 200 / (rayVector - rayStartVector).length());
        line[0].color = Color::Cyan;
        line[1].position = Vector2f(i, windowHeight / 2 - 200 / (rayVector - rayStartVector).length());
        line[1].color = Color::Magenta;

        window.draw(line, 2, sf::Lines);

        rayStartVector -= deltaPlane;
    }
}

void draw(Camera &camera, RenderWindow &window)
{
}

int main()
{
    RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "SFML works!");

    int posX = 4;
    int posY = 4;

    int dirX = 2;
    int dirY = 0;

    int plX = 0;
    int plY = -2;

    RectangleShape rect;
    rect.setSize(Vector2f(rectWidth, rectHeight));

    Camera camera(Vector2D(posX, posY), Vector2D(dirX, dirY), Vector2D(plX, plY));

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            //Подстраивается под процессор
            case (Event::KeyPressed):
                switch (event.key.code)
                {
                case (Keyboard::A):
                    camera.rotateLeft();
                    break;
                case (Keyboard::D):
                    camera.rotateRight();
                    break;
                case (Keyboard::W):
                    camera.moveForward();
                    break;
                case (Keyboard::S):
                    camera.moveBackward();
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

        drawWals(camera, window);
        drawMap(camera, rect, window);

        window.display();
    }
}