#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <SelbaWard/Line.hpp>
#include <vector>
#include <random>

std::ostream& operator<<(std::ostream& os_, const sf::Vector2f& vec_)
{
	os_ << '[' << vec_.x << ", " << vec_.y << ']';
	return os_;
}

struct FloatPair
{
	float first, second;
};

float map(float value_, FloatPair original_range_, FloatPair new_range_)
{
	return (value_- original_range_.first) / (original_range_.second - original_range_.first)
		* (new_range_.second - new_range_.first) + new_range_.first;
}

struct Branch : public sf::Drawable
{
	Branch(const Branch* parent_, float angle_)
		: parent{parent_}
		, order{parent->order + 1}
		, line{parent->line.getPoint(1)
			, parent->line.getPoint(1)
				+ sf::Transform{}
				.rotate(angle_ - 90 + parent->angle)
				.transformPoint({50 * static_cast<float>(std::pow(.9, order)), 0})
			, map(static_cast<float>(order), {0, 8}, {10, 1})
			, sf::Color{130, 219, 255}}
		, angle{angle_ + parent->angle}
	{
		line.setRounded(true);
	};

	Branch(sf::Vector2f position_)
		: parent{nullptr}
		, order{0}
		, line{{position_}, {position_}, map(static_cast<float>(order), {0, 8}, {10, 1}), sf::Color{130, 219, 255}}
		, angle{0}
	{
		line.setRounded(false);
	};

	void draw(sf::RenderTarget& target_, sf::RenderStates states_) const override
	{
		target_.draw(line);
	};

	const Branch* parent;
	int order;
	sw::Line line;
	float angle;
	float width;
};

class Tree
{
public:
	Tree(sf::Vector2f position_)
	{
		_branches.emplace_back(position_);

		for (auto i{0u}; i < _branches.size(); ++i)
		{
			std::mt19937 rng{std::random_device{}()};

			std::array<int, 2> angles;
			
			do
			{
				for (int& rand_angle : angles)
					rand_angle = std::uniform_int_distribution(-5 * (_branches[i].order + 1), 5 * (_branches[i].order + 1))(rng);
			}
			while (std::abs(angles[0] - angles[1]) < 5. * (_branches[i].order + 1));
					
			if (_branches[i].order < 7)
			{
				_branches.emplace_back(&_branches[i], angles[0]);     
				if (std::uniform_int_distribution{0, 10}(rng) < _branches[i].order)
					_branches.emplace_back(&_branches[i], angles[1]);     
			}
			else if (_branches[i].order == 7)
				_branches.emplace_back(&_branches[i], 45);
		}
	}

	auto begin() { return _branches.begin(); }
	auto end() {return _branches.end(); }

private:
	std::vector<Branch> _branches;
};

int main()
{
	sf::RenderWindow window({1900, 1000}, "SFML", sf::Style::Default, sf::ContextSettings{0, 0, 4});
	window.setFramerateLimit(24);

	std::vector<Tree> trees;

	for (float i{0}; i < 2; ++i)
		for (float j{0}; j < 4; ++j)
			trees.push_back({{450.f * j + 250.f, 400.f * (i + 1)}});

	while (true)
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
				return 0;
		}

		window.clear(sf::Color::White);
		for (auto branch : Tree{{500, 500}})
			window.draw(branch);
		window.display();
	}
}
