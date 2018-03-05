#include <Branch.hpp>
#include <Tree.hpp>
#include <cmath>
#include <Util.hpp>
#include <random>
#include <iostream>
#include <SFML/Graphics/ConvexShape.hpp>

Branch::Branch(int parent_index_, float angle_, const Tree& tree_)
	: parent_index{parent_index_}
	, tree{tree_}
	, order{tree[parent_index].order + 1}
	, line{tree[parent_index].line.getPoint(1)
		, tree[parent_index].line.getPoint(1)
			+ sf::Transform{}
			.rotate(angle_ - 90 + tree[parent_index].angle)
			.transformPoint({50 * static_cast<float>(std::pow(.9, order)), 0})
		, order < 8 ? map(static_cast<float>(order), {0, 8}, {10, 1}) : 12
		, {66, 35, 24}}
	, angle{angle_ + tree[parent_index].angle}
	, last_angle{0}
	, elapsed_frames{0}
{
	line.setRounded(true);
	std::mt19937 rng{std::random_device{}()};
	std::uniform_int_distribution<int> rand_length_mult{50, 150};
	line.setPoint(1, line.getPoint(0) + (line.getPoint(1) - line.getPoint(0)) * (static_cast<float>(rand_length_mult(rng)) / 100));
}

Branch::Branch(sf::Vector2f position_, const Tree& tree_)
	: parent_index{-1}
	, tree{tree_}
	, order{0}
	, line{{position_}, {position_ - sf::Vector2f{0, 70}}, map(static_cast<float>(order), {0, 8}, {10, 1}), {66, 35, 24}}
	, angle{0}
	, last_angle{0}
	, elapsed_frames{0}
{
	line.setRounded(false);
}

void Branch::update(float dt_)
{
	last_angle = angle;
	const auto vec{line.getPoint(1) - line.getPoint(0)};
	if (parent_index == -1)
	{
		angle = std::sin(static_cast<float>(elapsed_frames) * dt_) * 2.f;
		if (angle == 0)
			elapsed_frames = 0;
	}
	else
	{
		angle += tree[parent_index].angle - tree[parent_index].last_angle;
		line.setPoint(0, tree[parent_index].line.getPoint(1));
		line.setPoint(1, line.getPoint(0) + sf::Transform{}.rotate(tree[parent_index].angle - tree[parent_index].last_angle).transformPoint(vec));
	}

	++elapsed_frames;
}

void Branch::draw(sf::RenderTarget& target_, sf::RenderStates states_) const
{
	if (order < 8)
		target_.draw(line);
	else
	{
		sf::ConvexShape rect{4};
		rect.setPoint(0, {0, 0});
		rect.setPoint(1, {16, 0});
		rect.setPoint(2, {22, 10});
		rect.setPoint(3, {6, 10});
		rect.setFillColor({134, 255, 74, 80});
		rect.setOrigin({1, 1});
		rect.rotate(angle - 180);
		rect.setPosition(line.getPoint(0));
		target_.draw(rect);
	}
}
