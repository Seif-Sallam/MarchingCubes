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
    switch(state)
    {
        case AB:
        {
            //a
            output.first.position = topLeft;
            output.first.position.y += length / 2;
            //b
            output.second.position = topLeft;
            output.second.position.x += length / 2;
            output.second.position.y += length;
        }
        break;
        case BC:
        {
            // b
            output.first.position = topLeft;
            output.first.position.x += length / 2;
            output.first.position.y += length;
            // c
            output.second.position = topLeft;
            output.second.position += sf::Vector2f((float)length, (float)length / 2);
        }
        break;
        case CD:
        {
            //d
            output.first.position = topLeft;
            output.first.position.x += length / 2;
            //c
            output.second.position = topLeft;
            output.second.position += sf::Vector2f((float)length, (float)length / 2);
        }
        break;
        case DA:
        {
            //d
            output.first.position = topLeft;
            output.first.position.x += length / 2;
            //a
            output.second.position = topLeft;
            output.second.position.y += length / 2;
        }
        break;
        case AC:
        {
            //c
            output.first.position = topLeft;
            output.first.position += sf::Vector2f((float)length, (float)length / 2.0f);
            //a
            output.second.position = topLeft;
            output.second.position.y += length / 2;
        }
        break;
        case BD:
        {
            // b
            output.first.position = topLeft;
            output.first.position.x += length / 2;
            output.first.position.y += length;
            //d
            output.second.position = topLeft;
            output.second.position.x += length / 2;
        }
        break;
    }
    return output;
}

inline static std::vector<std::pair<sf::Vertex, sf::Vertex>>
getVerticies(const std::vector<std::vector<bool>>& grid, uint32_t topLeftX, uint32_t topLeftY, uint32_t length)
{
    bool topLeft = grid[topLeftY][topLeftX];
    bool topRight = grid[topLeftY + 1][topLeftX];
    bool bottomLeft = grid[topLeftY][topLeftX + 1];
    bool bottomRight = grid[topLeftY + 1][topLeftX + 1];

    uint32_t state = topLeft * 8 + topRight * 4 + bottomRight * 2 + bottomLeft;

    std::vector<std::pair<sf::Vertex, sf::Vertex>> verticies;

    sf::Vector2f topLeftPos = sf::Vector2f((float)topLeftX * (float)length, (float)topLeftY * (float)length);

    switch(state){
        case 1:
        {
            verticies.push_back(getLine(topLeftPos, length, AB));
        }
        break;
        case 2:
        {
            verticies.push_back(getLine(topLeftPos, length, BC));
        }
        break;
        case 3:
        {
            verticies.push_back(getLine(topLeftPos, length, AC));
        }
        break;
        case 4:
        {
            verticies.push_back(getLine(topLeftPos, length, CD));
        }
        break;
        case 5:
        {
            verticies.push_back(getLine(topLeftPos, length, DA));
            verticies.push_back(getLine(topLeftPos, length, BC));
        }
        break;
        case 6:
        {
            verticies.push_back(getLine(topLeftPos, length, BD));
        }
        break;
        case 7:
        {
            verticies.push_back(getLine(topLeftPos, length, DA));
        }
        break;
        case 8:
        {
            verticies.push_back(getLine(topLeftPos, length, DA));
        }
        break;
        case 9:
        {
            verticies.push_back(getLine(topLeftPos, length, BD));
        }
        break;
        case 10:
        {
            verticies.push_back(getLine(topLeftPos, length, AB));
            verticies.push_back(getLine(topLeftPos, length, CD));
        }
        break;
        case 11:
        {
            verticies.push_back(getLine(topLeftPos, length, CD));
        }
        break;
        case 12:
        {
            verticies.push_back(getLine(topLeftPos, length, AC));
        }
        break;
        case 13:
        {
            verticies.push_back(getLine(topLeftPos, length, BC));
        }
        break;
        case 14:
        {
            verticies.push_back(getLine(topLeftPos, length, AB));
        }
        break;
    }

    return verticies;
}

int main(int argc, char const *argv[])
{
    sf::RenderWindow window(sf::VideoMode(600, 600), "MarchingCubes");
    sf::VertexArray linesVA;

    linesVA.setPrimitiveType(sf::Lines);

    std::vector<sf::CircleShape> pointsVec;
    uint32_t cellSize = 20;
    uint32_t xCount = (window.getSize().x / cellSize) + 1;
    uint32_t yCount = (window.getSize().y / cellSize) + 1;

    pointsVec.resize(xCount * yCount);

    std::vector<std::vector<bool>> grid;

    grid.resize(yCount);
    for(auto& v : grid)
        v.resize(xCount);


    sf::Clock clk;

    std::random_device dv;
    std::mt19937 randomGen(dv());
    std::uniform_int_distribution uniformDist(0, 1);
    float dt = 0.0f;
    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        auto delta = clk.restart().asSeconds();
        dt += delta;
        if (dt <= 1.0f)
            continue;

        dt = 0.0f;
        for(uint32_t i = 0; i < yCount; i++)
            for (uint32_t j = 0; j < xCount; j++){
                grid[i][j] = (bool)uniformDist(randomGen);
                uint32_t index = j * yCount + i;

                sf::CircleShape& shape = pointsVec[index];
                if (grid[i][j])
                    shape.setFillColor(sf::Color::White);
                else
                    shape.setFillColor(sf::Color::Black);

                shape.setPosition(sf::Vector2f((float)cellSize * (float)j, (float)cellSize * (float)i));
                shape.setRadius(3.0f);
            }

        linesVA.clear();

        for(uint32_t i = 0; i < yCount - 1; i++)
            for (uint32_t j = 0; j < xCount - 1; j++){
                auto verticies = getVerticies(grid, j, i, cellSize);

                for(auto& v : verticies)
                {
                    linesVA.append(v.first);
                    linesVA.append(v.second);
                }
            }
        window.clear(sf::Color(167, 167, 167));
        for (auto& shape : pointsVec)
            window.draw(shape);

        window.draw(linesVA);
        window.display();
    }

    return 0;
}