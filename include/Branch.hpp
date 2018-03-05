#pragma once

#include <SelbaWard/Line.hpp>
#include <SFML/Graphics/Drawable.hpp>

class Tree;

struct Branch : public sf::Drawable
{
	Branch(int parent_index_, float angle_, const Tree& tree_);
	Branch(sf::Vector2f position_, const Tree& tree_);
	void update(float dt_);
	void draw(sf::RenderTarget& target_, sf::RenderStates states_) const override;

	void setRandomAngleMultiplier();

	int parent_index;
	const Tree& tree;
	int order;
	sw::Line line;
	float angle, last_angle;
	float width;
	int elapsed_frames;
	float angle_multiplier;
};
