#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <SelbaWard/Line.hpp>

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

float length(sf::Vector2f vec_)
{
	return std::sqrt(std::pow(vec_.x, 2) + std::pow(vec_.y, 2));
}

struct Branch : public sf::Drawable
{
	Branch(const Branch* parent_, float angle_)
		: parent{parent_}
		, order{parent_->order + 1}
		, line{parent_->line.getPoint(1)
			, parent_->line.getPoint(1)
				+ sf::Transform{}
				.rotate(angle_ - 90 + parent_->angle)
				.transformPoint({50 * static_cast<float>(std::pow(.9, order)), 0})
			, map(order, {0, 8}, {10, 1})
			, sf::Color::Black}
		, angle{angle_}
	{
		line.setRounded(true);
	};

	Branch()
		: parent{nullptr}
		, order{0}
		// , length{map(order, {0, 8}, {100, 10})}
		, line{{500, 500}, {500, 450}, map(order, {0, 8}, {10, 1}), sf::Color::Black}
		, angle{0}
	{
		line.setRounded(true);
	};

	void draw(sf::RenderTarget& target_, sf::RenderStates states_) const override
	{
		target_.draw(line);
	};

	int order;
	const Branch* parent;
	sw::Line line;
	float angle;
	float width;
};

int main()
{
	sf::RenderWindow window({900, 900}, "SFML");

	Branch branch;
	Branch branch2{&branch, 20};
	Branch branch3{&branch2, 20};

	// std::cout << branch2.start;

	while (true)
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				return 0;
		}

		window.clear(sf::Color::White);
		window.draw(branch);
		window.draw(branch2);
		window.draw(branch3);
		window.display();
	}
}
