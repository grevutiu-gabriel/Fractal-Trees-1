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

#include <Tree.hpp>
#include <Branch.hpp>
#include <random>
#include <array>

namespace frt
{

Tree::Tree(sf::Vector2f position_)
{
	branches.emplace_back(position_, *this);

	for (auto i{0u}; i < branches.size(); ++i)
	{
		std::mt19937 rng{std::random_device{}()};

		std::array<int, 2> angles;
		
		for (int& rand_angle : angles)
			rand_angle = std::uniform_int_distribution{20, 40}(rng);
		
		angles[0] *= -1;

		if (branches[i].getOrder() < 7)
		{
			int rand{std::uniform_int_distribution{1, 11}(rng)};

			if (rand  > branches[i].getOrder())
			{
				branches.emplace_back(i, branches.size() - 1, angles[0], *this);     
				branches.emplace_back(i, branches.size() - 1, angles[1], *this);     
			}
			else if (std::uniform_int_distribution{0, 1}(rng))
				branches.emplace_back(i, branches.size() - 1, angles[0], *this);     
			else
				branches.emplace_back(i, branches.size() - 1, angles[1], *this);     
		}
		else if (branches[i].getOrder() == 7)
			branches.emplace_back(i, branches.size() - 1, 45, *this);
	}
}

} // frt
