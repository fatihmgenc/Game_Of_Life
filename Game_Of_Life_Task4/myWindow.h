#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;

class myWindow 
{
public:
        sf::Text text1;
        sf::RenderWindow window;

    myWindow() {
        sf::RenderWindow window(sf::VideoMode(200, 200), "This window created by myWindow class!");
        sf::Font font;
        if (!font.loadFromFile("TimesRoman.ttf"))
        {
            // error...
        }
        text1.setString("Hello AdvC++");
        text1.setFont(font);
        text1.setCharacterSize(12);
        text1.setStyle(sf::Text::Regular);

        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            window.clear();
            window.draw(text1);
            window.display();
        }
    }

public:
    void setText(char string1) {
        text1.setString(text1.getString()+string1);
    }
   
    void PrintTable(vector<vector<bool>> GameTable) {
        std::cout << "print table";
        window.clear();
        text1.setString("Game Table : \n");
        for (int i = 0; i < GameTable.size(); i++) {
            for (int j = 0; j < GameTable[0].size(); j++) {
                if (GameTable[i][j])
                    text1.setString(text1.getString() + "1");
                else
                    text1.setString(text1.getString() + " 0");
            }
            text1.setString(text1.getString() + "\n");
        }

        window.draw(text1);
        window.display();

            
    }

    
};

