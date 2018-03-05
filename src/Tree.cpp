#include <Tree.hpp>
#include <Branch.hpp>
#include <random>
#include <array>

Tree::Tree(sf::Vector2f position_)
{
	_branches.emplace_back(position_, *this);

	for (auto i{0u}; i < _branches.size(); ++i)
	{
		std::mt19937 rng{std::random_device{}()};

		std::array<int, 2> angles;
		
		for (int& rand_angle : angles)
			rand_angle = std::uniform_int_distribution(20, 40)(rng);
		
		angles[0] *= -1;

		if (_branches[i].order < 7)
		{
			_branches.emplace_back(i, angles[0], *this);     
			if (std::uniform_int_distribution{0, 10}(rng) > _branches[i].order)
				_branches.emplace_back(i, angles[1], *this);     
		}
		else if (_branches[i].order == 7)
			_branches.emplace_back(i, 45, *this);
	}
}