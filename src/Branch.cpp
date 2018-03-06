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
	, elapsed_frames{tree[parent_index].elapsed_frames}
	, angle_multiplier{1}
{
	line.setRounded(true);
	std::mt19937 rng{std::random_device{}()};
	int rand_length_percent{std::uniform_int_distribution<int>{66, 150}(rng)};
	line.setPoint(1, line.getPoint(0) + (line.getPoint(1) - line.getPoint(0)) * (static_cast<float>(rand_length_percent) / 100));
	setRandomAngleMultiplier();
}

Branch::Branch(sf::Vector2f position_, const Tree& tree_)
	: parent_index{-1}
	, tree{tree_}
	, order{0}
	, line{{position_}, {position_ - sf::Vector2f{0, 70}}, map(static_cast<float>(order), {0, 8}, {10, 1}), {66, 35, 24}}
	, angle{0}
	, last_angle{0}
	, elapsed_frames{0}
	, angle_multiplier{1}
{
	setRandomAngleMultiplier();

	std::mt19937 rng{std::random_device{}()};
	int rand_start_elapsed_headstart{std::uniform_int_distribution<int>{0, 144}(rng)};
	elapsed_frames = rand_start_elapsed_headstart;
}

void Branch::update(float dt_)
{
	last_angle = angle;
	
	const auto vec{line.getPoint(1) - line.getPoint(0)};

	if (parent_index == -1)
		angle = std::sin(static_cast<float>(elapsed_frames) * dt_) * 2.f * angle_multiplier;
	else
	{
		float angle_change{(tree[parent_index].angle - tree[parent_index].last_angle) / static_cast<float>(order) * 2};

		angle += angle_change;
		line.setPoint(0, tree[parent_index].line.getPoint(1));
		line.setPoint(1, line.getPoint(0) + sf::Transform{}.rotate(angle_change).transformPoint(vec));
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

void Branch::setRandomAngleMultiplier()
{
	std::mt19937 rng{std::random_device{}()};
	int rand_angle_percent{std::uniform_int_distribution<int>{50, 150}(rng)};
	angle_multiplier = static_cast<float>(rand_angle_percent) / 100.f;
}
