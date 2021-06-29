#include <iostream>
using namespace std;

#include <SFML/Graphics.hpp>
using namespace sf;

#include <cmath>
#include "engine.h"
#include "configurations.cpp"

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

//Отрисовка карты, сейчас удалю, потом надо восстановить
void drawMap(const Camera &camera, RectangleShape &rect, RenderWindow &window)
{
    rect.setSize(Vector2f(1, 1));
    rect.setFillColor(Color::Green);
    rect.setPosition(camera.position.x * rectWidth, camera.position.y * rectHeight);
    window.draw(rect);

    rect.setFillColor(Color::Yellow);
    rect.setPosition((camera.position.x + camera.direction.x) * rectWidth, (camera.position.y + camera.direction.y) * rectHeight);
    window.draw(rect);

    rect.setSize(Vector2f(1, 1));
    rect.setFillColor(Color::Red);

    for (int i = 0; i < windowWidth; ++i)
    {
        float cameraX = -(2 * i / double(windowWidth) - 1);
        Vector2D rayDir = camera.direction + camera.plane * cameraX;
        Vector2D dRay = rayDir;
        Vector2D ray = camera.position + rayDir;

        float screenZ = 0.0f;
        rect.setFillColor(Color(200, 100, 196));
        for (int j = windowHeight; j > windowHeight / 2 - 1; --j)
        {
            float maxLength = screenZ * rayDir.length() / (camera.z - screenZ);
            while ((ray - camera.position).length() < maxLength)
            {
                ray += dRay;
            }
            rect.setPosition(ray.x * rectWidth, ray.y * rectHeight);
            window.draw(rect);
            screenZ += deltaScreenZ;
        }
    }

    rect.setSize(Vector2f(rectWidth, rectHeight));
}

//Хороший эффект для Cursed Spaces, но сейчас удалю
void drawFloor(Camera &camera, RenderWindow &window, RectangleShape &rect)
{
    Vertex line[2];
    for (int i = 0; i < windowWidth; ++i)
    {
        float cameraX = -(2 * i / double(windowWidth) - 1);
        Vector2D rayDir = camera.direction + camera.plane * cameraX;
        //Vector2D dRay = (rayDir / 20.f);

        for (int j = windowHeight; j > windowHeight / 2; --j)
        {
            Vector2D dRay = rayDir * 5.f / j;
            //Vector2D dRay = rayDir * (windowHeight - j) / (0.5f - windowHeight + j);
            rect.setFillColor(Color((rayDir).length() * 100, 0, 225));
            rect.setPosition((camera.position.x + rayDir.x) * rectWidth, (camera.position.y + rayDir.y) * rectHeight);
            window.draw(rect);
            int mapX = int(camera.position.x + rayDir.x);
            int mapY = int(camera.position.y + rayDir.y);
            if (world[mapX][mapY] == 1)
            {
                line[0].position = Vector2f(i, j);
                line[0].color = Color::Cyan;
                line[1].position = Vector2f(i, windowHeight - j);
                line[1].color = Color::Magenta;
                window.draw(line, 2, sf::Lines);
                j = windowHeight / 2;
            }
            else if (mapX % 2 == 0 && mapY % 2 == 0)
            {
                rect.setFillColor(Color::Cyan);
            }
            else
            {
                rect.setFillColor(Color::Blue);
            }
            rect.setPosition(i, j);
            window.draw(rect);
            rayDir += dRay;
        }
    }
}

void floorCast(Camera &camera, RenderWindow &window, RectangleShape &rect)
{
    RectangleShape line;
    for (int i = 0; i < windowWidth; ++i)
    {
        float cameraX = -(2 * i / double(windowWidth) - 1);
        Vector2D rayDir = camera.direction + camera.plane * cameraX;
        Vector2D dRay = rayDir / 10.f;
        Vector2D ray = camera.position + rayDir;

        float screenZ = 0.0f;
        for (int j = windowHeight; j > windowHeight / 2; --j)
        {
            float maxLength = screenZ * rayDir.length() / (camera.z - screenZ);
            while ((ray - camera.position).length() < maxLength && world[int(ray.x)][int(ray.y)] == 0)
            {
                ray += dRay;
            }
            //Чтобы не отрисовывать то, что находится за стенами, для бесконечной плоскости неактуально
            if(world[int(ray.x)][int(ray.y)] != 0)
            {
                break;
            }
            //WallCast, надо доделать потом
            /*
            if (world[int(ray.x)][int(ray.y)] != 0)
            {
                line.setFillColor(Color::Blue);
                float wallSize = dRay.length() * windowHeight / ray.length();
                cout<<wallSize<<endl;
                line.setSize(Vector2f(1, wallSize));
                line.setPosition(i, j - wallSize);
                window.draw(line);
                j = windowHeight / 2;
            }
            */
            if (int(ray.x) % 2 == 0 && int(ray.y) % 2 == 0)
            {
                rect.setFillColor(Color::Cyan);
            }
            else
            {
                rect.setFillColor(Color::White);
            }

            rect.setPosition(i, j);
            window.draw(rect);
            screenZ += deltaScreenZ;
        }
    }
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

int main()
{
    RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "SFML works!");

    setWorld();

    float posX = 4;
    float posY = 4;

    float dirX = 0.5f;
    float dirY = 0;

    float plX = 0;
    float plY = -0.8f;

    RectangleShape rect;
    rect.setSize(Vector2f(rectWidth, rectHeight));

    Camera camera(Vector2D(posX, posY), Vector2D(dirX, dirY), Vector2D(plX, plY));
    camera.z = 0.5f;

    RectangleShape rectForFloor;
    rectForFloor.setSize(Vector2f(1, 1));

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

        floorCast(camera, window, rectForFloor);

        window.display();
    }
}