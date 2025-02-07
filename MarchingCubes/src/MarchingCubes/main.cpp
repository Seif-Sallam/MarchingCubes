#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>


enum LINE_STATE{
    AB = 0,
    BC,
    CD,
    DA,
    AC,
    BD,
};

uint32_t cellSize = 15;
uint32_t xCount = 0;
uint32_t yCount = 0;

std::random_device dv;
std::mt19937 randomGen(dv());
std::uniform_int_distribution uniformDist(0, 1);

std::vector<std::vector<bool>> grid;

inline static std::pair<sf::Vertex, sf::Vertex>
getLine(sf::Vector2f topLeft, uint32_t length, LINE_STATE state)
{
    /*
            d
        *--------*
        \        \
       a\        \ c
        *--------*
            b
        case 0: ab
        case 1: bc
        case 2: cd
        case 3: da
        case 4: ac
        case 5: bd
    */


    std::pair<sf::Vertex, sf::Vertex> output;
    output.first.color = sf::Color::White;
    output.second.color = sf::Color::White;
    sf::Vector2f a = topLeft + sf::Vector2f(0.0f, length / 2.0f);
    sf::Vector2f b = topLeft + sf::Vector2f((float)length / 2.0f, (float)length);
    sf::Vector2f c = topLeft + sf::Vector2f((float)length, length / 2.0f);
    sf::Vector2f d = topLeft + sf::Vector2f(length / 2.0f, 0.0f);
    switch(state)
    {
        case AB:
        {
            //a
            output.first.position = a;
            //b
            output.second.position = b;
        }
        break;
        case BC:
        {
            // b
            output.first.position = b;
            // c
            output.second.position = c;
        }
        break;
        case CD:
        {
            //d
            output.first.position = d;
            //c
            output.second.position = c;
        }
        break;
        case DA:
        {
            //d
            output.first.position = d;
            //a
            output.second.position = a;
        }
        break;
        case AC:
        {
            //c
            output.first.position = c;
            //a
            output.second.position = a;
        }
        break;
        case BD:
        {
            // b
            output.first.position = b;
            //d
            output.second.position = d;
        }
        break;
    }
    return output;
}

inline static std::vector<std::pair<sf::Vertex, sf::Vertex>>
getVertices(uint32_t topLeftX, uint32_t topLeftY, uint32_t length)
{
    bool topLeft = grid[topLeftY][topLeftX];
    bool topRight = grid[topLeftY][topLeftX + 1];
    bool bottomLeft = grid[topLeftY + 1][topLeftX];
    bool bottomRight = grid[topLeftY + 1][topLeftX + 1];

    uint32_t state = topLeft * 8 + topRight * 4 + bottomRight * 2 + bottomLeft * 1;

    std::vector<std::pair<sf::Vertex, sf::Vertex>> vertices;

    sf::Vector2f topLeftPos = sf::Vector2f((float)topLeftX * (float)length, (float)topLeftY * (float)length);

    switch(state){
        case 1:
        {
            vertices.push_back(getLine(topLeftPos, length, AB));
        }
        break;
        case 2:
        {
            vertices.push_back(getLine(topLeftPos, length, BC));
        }
        break;
        case 3:
        {
            vertices.push_back(getLine(topLeftPos, length, AC));
        }
        break;
        case 4:
        {
            vertices.push_back(getLine(topLeftPos, length, CD));
        }
        break;
        case 5:
        {
            vertices.push_back(getLine(topLeftPos, length, CD));
            vertices.push_back(getLine(topLeftPos, length, AB));
        }
        break;
        case 6:
        {
            vertices.push_back(getLine(topLeftPos, length, BD));
        }
        break;
        case 7:
        {
            vertices.push_back(getLine(topLeftPos, length, DA));
        }
        break;
        case 8:
        {
            vertices.push_back(getLine(topLeftPos, length, DA));
        }
        break;
        case 9:
        {
            vertices.push_back(getLine(topLeftPos, length, BD));
        }
        break;
        case 10:
        {
            vertices.push_back(getLine(topLeftPos, length, AB));
            vertices.push_back(getLine(topLeftPos, length, CD));
        }
        break;
        case 11:
        {
            vertices.push_back(getLine(topLeftPos, length, CD));
        }
        break;
        case 12:
        {
            vertices.push_back(getLine(topLeftPos, length, AC));
        }
        break;
        case 13:
        {
            vertices.push_back(getLine(topLeftPos, length, BC));
        }
        break;
        case 14:
        {
            vertices.push_back(getLine(topLeftPos, length, AB));
        }
        break;
    }

    return vertices;
}

void populateGrid(std::vector<sf::CircleShape>& pointsVec)
{
 for(uint32_t i = 0; i < yCount; i++)
    for (uint32_t j = 0; j < xCount; j++){
        grid[i][j] = (bool)uniformDist(randomGen);
        uint32_t index = j * yCount + i;

        sf::CircleShape& shape = pointsVec[index];
        if (grid[i][j])
            shape.setFillColor(sf::Color::White);
        else
            shape.setFillColor(sf::Color::Black);

        shape.setRadius(2.0f);
        shape.setOrigin(sf::Vector2f(shape.getRadius() / 2.0f, shape.getRadius() / 2.0f));
        shape.setPosition(sf::Vector2f((float)cellSize * (float)j, (float)cellSize * (float)i));
    }
}

void populateLinesMarchingCubes(sf::VertexArray& va)
{
    for(uint32_t i = 0; i < yCount - 1; i++)
        for (uint32_t j = 0; j < xCount - 1; j++){
            auto vertices = getVertices(j, i, cellSize);
            for(auto& v : vertices)
            {
                va.append(v.first);
                va.append(v.second);
            }
        }
}

int main(int argc, char const *argv[])
{
    sf::RenderWindow window(sf::VideoMode(600, 600), "MarchingCubes");
    xCount = (window.getSize().x / cellSize) + 1;
    yCount = (window.getSize().y / cellSize) + 1;

    sf::VertexArray linesVA;

    linesVA.setPrimitiveType(sf::Lines);

    std::vector<sf::CircleShape> pointsVec;

    pointsVec.resize(xCount * yCount);
    grid.resize(yCount);
    for(auto& v : grid)
        v.resize(xCount);
    sf::Clock clk;

    float dt = 2.0f;


    bool paused = false;
    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P)
                paused = !paused;
        }

        auto delta = clk.restart().asSeconds();
        dt += delta;
        if (dt <= 1.0f || paused)
            continue;

        dt = 0.0f;

        populateGrid(pointsVec);

        linesVA.clear();
        populateLinesMarchingCubes(linesVA);

        window.clear(sf::Color(167, 167, 167));
        for (auto& shape : pointsVec)
            window.draw(shape);

        window.draw(linesVA);
        window.display();
    }

    return 0;
}