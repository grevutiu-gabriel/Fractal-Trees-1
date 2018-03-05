#pragma once

#include <Branch.hpp>

class Tree
{
public:
	Tree(sf::Vector2f position_);

	auto begin() { return _branches.begin(); }
	auto end() {return _branches.end(); }
	auto operator[](int index_) const { return _branches[index_]; }

private:
	std::vector<Branch> _branches;
};
