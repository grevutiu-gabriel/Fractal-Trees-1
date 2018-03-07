////////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (c) 2018, Kim Schmider
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
////////////////////////////////////////////////////////////////////////////////

#include <Branch.hpp>
#include <Tree.hpp>
#include <cmath>
#include <Util.hpp>
#include <random>
#include <iostream>
#include <SFML/Graphics/ConvexShape.hpp>
#include <Leaf.hpp>

namespace frt
{

Branch::Branch(int parent_index_, int current_index_, float angle_, Tree& tree_)
	: parent_index{parent_index_}
	, child_index{-1}
	, tree{tree_}
	, order{tree[parent_index].order + 1}
	, line{tree[parent_index].getEnd()
		, tree[parent_index].getEnd()
			+ sf::Transform{}
			.rotate(angle_ - 90 + tree[parent_index].angle)
			.transformPoint({50 * static_cast<float>(std::pow(.9, order)), 0})
		, _util::map(static_cast<float>(order), {0, 8}, {10, 1})
		, {66, 35, 24}}
	, angle{angle_ + tree[parent_index].angle}
	, last_angle{0}
	, elapsed_frames{tree[parent_index].elapsed_frames}
	, angle_multiplier{1}
	, rng{tree[parent_index].rng}
{
	line.setRounded(true);
	
	int rand_length_percent{std::uniform_int_distribution<int>{66, 150}(rng)};
	line.setPoint(1, getStart() + (getEnd() - getStart()) * (static_cast<float>(rand_length_percent) / 100));
	setRandomAngleMultiplier();

	tree_[parent_index].child_index = current_index_;
}

Branch::Branch(sf::Vector2f position_, const Tree& tree_)
	: parent_index{-1}
	, child_index{-1}
	, tree{tree_}
	, order{0}
	, line{{position_}, {position_ - sf::Vector2f{0, 70}}, _util::map(static_cast<float>(order), {0, 8}, {10, 1}), {66, 35, 24}}
	, angle{0}
	, last_angle{0}
	, elapsed_frames{0}
	, angle_multiplier{1}
	, rng{std::random_device{}()}
{
	setRandomAngleMultiplier();

	std::mt19937 rng{std::random_device{}()};
	int rand_start_elapsed_headstart{std::uniform_int_distribution<int>{0, 144}(rng)};
	elapsed_frames = rand_start_elapsed_headstart;
}

void Branch::update(float dt_)
{
	last_angle = angle;
	
	const auto vec{getEnd() - getStart()};

	if (parent_index == -1)
		angle = std::sin(static_cast<float>(elapsed_frames) * dt_) * 2.f * angle_multiplier;
	else
	{
		float angle_change{(tree[parent_index].angle - tree[parent_index].last_angle) / static_cast<float>(order) * 2};

		angle += angle_change;
		line.setPoint(0, tree[parent_index].getEnd());
		line.setPoint(1, getStart() + sf::Transform{}.rotate(angle_change).transformPoint(vec));
	}

	++elapsed_frames;
}

void Branch::draw(sf::RenderTarget& target_, sf::RenderStates states_) const
{
	target_.draw(line);

	if (order > 2)
	{
		float angle1{}
			, angle2{angle};

		if (child_index != -1)
		{
			angle1 = angle2 = tree[child_index].angle;
			angle1 -= 70;
			angle2 -= 160;
		}
		else
		{
			angle1 = angle2 = angle;
			angle1 -= 70;
			angle2 -= 160;
		}

		Leaf leaf1{*this, angle1, rng}
			, leaf2{*this, angle2, rng};

		target_.draw(leaf1);
		target_.draw(leaf2);
	}

}

void Branch::setRandomAngleMultiplier()
{
	std::mt19937 rng{std::random_device{}()};
	int rand_angle_percent{std::uniform_int_distribution<int>{50, 150}(rng)};
	angle_multiplier = static_cast<float>(rand_angle_percent) / 100.f;
}

} // frt
