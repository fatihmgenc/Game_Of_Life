#include <SFML/Graphics.hpp>


class SingletonWindow {
  
protected:
    static SingletonWindow* instance;
    SingletonWindow() {
        sf::RenderWindow window(sf::VideoMode(200, 200), "This window created by myWindow class!");
        sf::Font font;
        if (!font.loadFromFile("TimesRoman.ttf"))
        {
            //error...
        }
        text1.setString("Hello AdvC++");
        text1.setFont(font);
        text1.setCharacterSize(12);
        text1.setStyle(sf::Text::Regular);
    }
public:
    sf::Text text1;
    sf::RenderWindow window;
    sf::Event event;
public:
    static SingletonWindow* getInstance() {
        if (!instance)
            instance = new SingletonWindow;
        return instance;
    }

};


