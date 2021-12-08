#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>

struct ExtendedShape {
    ExtendedShape() {};

    ExtendedShape(std::shared_ptr<sf::Shape> shape, sf::Text sf_text, float sx = 0, float sy = 0) :
        shape(shape), sf_text(sf_text), sx(sx), sy(sy) 
    {

    }

    std::shared_ptr<sf::Shape> shape;
    sf::Text sf_text;
    float sx = 0;
    float sy = 0;
};

int main(int argc, char * argv[])
{
    std::vector<ExtendedShape> shapes;

    int wWidth = 640;
    int wHeight = 480;
    sf::Font myFont;

    std::string name;
    float x, y, sx, sy;
    int r, g, b;
    float w, h;
    float radius;

    std::ifstream fin("src/config.txt");
    std::string type;
    while (fin >> type) {
        if (type == "Window") 
        {
            fin >> wWidth;
            fin >> wHeight;
        }
        else if (type == "Font") 
        {
            std::string fileName;
            fin >> fileName;
            if (!myFont.loadFromFile(fileName))
            {
                std::cerr << "Could not load font." << std::endl;
            }
        }
        else if (type == "Circle") 
        {
            fin >> name >> x >> y >> sx >> sy >> r >> g >> b >> radius;

            std::shared_ptr<sf::CircleShape> circle = std::make_shared<sf::CircleShape>();
            circle->setPosition(x, y);
            circle->setRadius(radius);
            circle->setFillColor(sf::Color(r, g, b));

            ExtendedShape newShape(circle, sf::Text(name, myFont, 24), sx, sy);

            shapes.push_back(newShape);
        }
        else if (type == "Rectangle")
        {
            fin >> name >> x >> y >> sx >> sy >> r >> g >> b >> w >> h;

            std::shared_ptr<sf::RectangleShape> rect = std::make_shared<sf::RectangleShape>();
            rect->setPosition(x, y);
            rect->setSize(sf::Vector2f(w, h));
            rect->setFillColor(sf::Color(r, g, b));

            ExtendedShape newShape(rect, sf::Text(name, myFont, 24), sx, sy);

            shapes.push_back(newShape);
        }
    }

    sf::RenderWindow window(sf::VideoMode(wWidth, wHeight), "SFML works!");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        window.clear();

        // Update all shape positions and draw them
        for (ExtendedShape& eShape : shapes)
        {
            // If structure to check if object hits window bounds
            if (eShape.shape->getPosition().x <= 0) 
            {
                eShape.sx *= -1;
            }

            if (eShape.shape->getPosition().y <= 0)
            {
                eShape.sy *= -1;
            }

            if (eShape.shape->getPosition().x + eShape.shape->getLocalBounds().width >= window.getSize().x)
            {
                eShape.sx *= -1;
            }

            if (eShape.shape->getPosition().y + eShape.shape->getLocalBounds().height >= window.getSize().y)
            {
                eShape.sy *= -1;
            }

            eShape.shape->setPosition(eShape.shape->getPosition() + sf::Vector2f(eShape.sx, eShape.sy));

            // Set text in the middle of the object
            eShape.sf_text.setPosition((eShape.shape->getPosition().x + eShape.shape->getLocalBounds().width / 2) - (eShape.sf_text.getLocalBounds().width / 2),
                                       (eShape.shape->getPosition().y + eShape.shape->getLocalBounds().height / 2) - (eShape.sf_text.getCharacterSize() / 2) );

            window.draw(*eShape.shape);
            window.draw(eShape.sf_text);
        }

        window.display();
    }

    return 0;
}