#include <Tree.hpp>
#include <Branch.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

int main()
{
	sf::RenderWindow window({1900, 1000}, "SFML", sf::Style::Default, sf::ContextSettings{0, 0, 4});
	sf::VertexArray background{sf::Quads, 4};
	background[0].position = {0, 0};
	background[1].position = {1900, 0};
	background[2].position = {1900, 1000};
	background[3].position = {0, 1000};

	background[0].color = background[1].color = {235, 129, 61};
	background[2].color = background[3].color = {235, 63, 103};

	std::vector<frt::Tree> trees;
	trees.reserve(8);

	for (float i{0}; i < 2; ++i)
		for (float j{0}; j < 4; ++j)
			trees.emplace_back(sf::Vector2f{450.f * j + 250.f, 400.f * (i + 1)});

	sf::Clock clock;
	float lag{0.f};
	const float delta_time{0.0069f};
	while (true)
	{
		sf::Time elapsed = clock.restart();
		lag += elapsed.asSeconds();

		while (lag >= delta_time)
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
					return 0;
			}

			for (auto& tree : trees)
				for (auto& branch : tree)
					branch.update(delta_time);

			lag -= delta_time;	
		}


		window.clear(sf::Color::White);
		window.draw(background);
		for (auto& tree : trees)
			for (const auto& branch : tree)
				window.draw(branch);
		window.display();
	}
}
