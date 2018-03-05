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
			, order < 8 ? map(static_cast<float>(order), {0, 8}, {10, 1}) : 12
			, sf::Color::Black}
		, angle{angle_ + parent->angle}
	{
		line.setRounded(true);
		line.setColor(sf::Color{66, 35, 24});
	};

	Branch()
		: parent{nullptr}
		, order{0}
		, line{{500, 500}, {500, 430}, map(static_cast<float>(order), {0, 8}, {10, 1}), sf::Color::Black}
		, angle{0}
	{
		line.setRounded(false);
		line.setColor({66, 35, 24});
	};

	void draw(sf::RenderTarget& target_, sf::RenderStates states_) const override
	{
		if (order < 8)
			target_.draw(line);
		else
		{
			sf::RectangleShape rect{{16, 16}};
			rect.setFillColor({186, 40, 67, 120});
			rect.setOrigin({8, 8});
			rect.rotate(angle);
			rect.setPosition(line.getPoint(0));
			target_.draw(rect);
		}
	};

	const Branch* parent;
	int order;
	sw::Line line;
	float angle;
	float width;
};

int main()
{
	sf::RenderWindow window({900, 900}, "SFML", sf::Style::Default, sf::ContextSettings{0, 0, 4});

	std::vector<Branch> tree;
	tree.reserve(8);
	tree.emplace_back();

	for (auto i{0u}; i < tree.size(); ++i)
	{
		std::mt19937 rng{std::random_device{}()};

		std::array<int, 2> angles;
		
		do
		{
			for (int& rand_angle : angles)
				rand_angle = std::uniform_int_distribution(-40, 40)(rng);
		}
		while (std::abs(angles[0] - angles[1]) < 40);
				
		if (tree[i].order < 7)
		{
			tree.emplace_back(&tree[i], angles[0]);     
			if (std::uniform_int_distribution{0, 10}(rng) > tree[i].order)
				tree.emplace_back(&tree[i], angles[1]);     
		}
		else if (tree[i].order == 7)
			tree.emplace_back(&tree[i], 45);
	}

	while (true)
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
				return 0;
		}

		window.clear(sf::Color::White);
		for (const auto& branch : tree)
			window.draw(branch);
		window.display();
	}
}
