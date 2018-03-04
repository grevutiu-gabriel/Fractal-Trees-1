#include <SFML/Graphics.hpp>

int main()
{
	sf::RenderWindow window({900, 900}, "SFML");

	while (true)
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				return 0;
		}

		window.clear();
		// window.draw();
		window.display();
	}
}
