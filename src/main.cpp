#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <SelbaWard/Line.hpp>
#include <vector>

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
		, order{parent_->order + 1}
		, line{parent_->line.getPoint(1)
			, parent_->line.getPoint(1)
				+ sf::Transform{}
				.rotate(angle_ - 90 + parent_->angle)
				.transformPoint({50 * static_cast<float>(std::pow(.9, order)), 0})
			, order < 8 ? map(order, {0, 8}, {10, 1}) : 12
			, sf::Color::Black}
		, angle{angle_ + parent_->angle}
	{
		line.setRounded(true);
		line.setColor(order < 8 ? sf::Color{66, 35, 24} : sf::Color{186, 40, 67, 100});
	};

	Branch()
		: parent{nullptr}
		, order{0}
		, line{{500, 500}, {500, 450}, map(order, {0, 8}, {10, 1}), sf::Color::Black}
		, angle{0}
	{
		line.setRounded(true);
		line.setColor({66, 35, 24});
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
	sf::RenderWindow window({900, 900}, "SFML", sf::Style::Default, sf::ContextSettings{0, 0, 4});

	std::vector<Branch> tree;
	tree.reserve(8);
	tree.emplace_back();

	for (int i; i < 255; ++i)
		for (int angle : {30, -15})
			tree.emplace_back(&tree[i], angle);

	while (true)
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				return 0;
		}

		window.clear(sf::Color::White);
		for (const auto& branch : tree)
			window.draw(branch);
		window.display();
	}
}
