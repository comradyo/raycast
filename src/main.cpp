#include <iostream>
using namespace std;

#include <SFML/Graphics.hpp>
using namespace sf;

#include <cmath>
#include "engine.h"
#include "configurations.cpp"

char world[worldWidth][worldHeight];

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

void draw(Camera &camera, RenderWindow &window)
{
}

int main()
{
    RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "SFML works!");

    setWorld();

    int posX = 4;
    int posY = 4;

    float dirX = 0.5f;
    float dirY = 0;

    float plX = 0;
    float plY = -0.3f;

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