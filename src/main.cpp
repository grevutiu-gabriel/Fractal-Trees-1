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
			, {66, 35, 24}}
		, angle{angle_ + parent->angle}
		, last_angle{0}
		, elapsed_frames{0}
	{
		line.setRounded(true);
		std::mt19937 rng{std::random_device{}()};
		std::uniform_int_distribution<int> rand_length_mult{50, 150};
		line.setPoint(1, line.getPoint(0) + (line.getPoint(1) - line.getPoint(0)) * (static_cast<float>(rand_length_mult(rng)) / 100));
	};

	Branch(sf::Vector2f position_)
		: parent{nullptr}
		, order{0}
		, line{{position_}, {position_ - sf::Vector2f{0, 70}}, map(static_cast<float>(order), {0, 8}, {10, 1}), {66, 35, 24}}
		, angle{0}
		, last_angle{0}
		, elapsed_frames{0}
	{
		line.setRounded(false);
	};

	void update(float dt_)
	{
		if (parent == nullptr)
		{
			auto vec{line.getPoint(1) - line.getPoint(0)};
			last_angle = angle;
			angle = std::sin(static_cast<float>(elapsed_frames) * dt_ * 3.14159f) * 2.f;
			if (angle == 0)
				elapsed_frames = 0;
			std::cout << angle << '\n';
			line.setPoint(1, line.getPoint(0) + sf::Transform{}.rotate(angle - last_angle).transformPoint(vec));
		}
		else
		{
			line.setPoint(0, parent->line.getPoint(0));
		}

		++elapsed_frames;
	};

	void draw(sf::RenderTarget& target_, sf::RenderStates states_) const override
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
	};

	const Branch* parent;
	int order;
	sw::Line line;
	float angle, last_angle;
	float width;
	int elapsed_frames;
};

class Tree
{
public:
	Tree(sf::Vector2f position_)
	{
		_branches.emplace_back(position_);

		for (auto i{0u}; i < _branches.size(); ++i)
		{
			std::mt19937 rng{std::random_device{}()};

			std::array<int, 2> angles;
			
			do
			{
				for (int& rand_angle : angles)
					rand_angle = std::uniform_int_distribution(-40, 40)(rng);
			}
			while (std::abs(angles[0] - angles[1]) < 40);
					
			if (_branches[i].order < 7)
			{
				_branches.emplace_back(&_branches[i], angles[0]);     
				if (std::uniform_int_distribution{0, 10}(rng) > _branches[i].order)
					_branches.emplace_back(&_branches[i], angles[1]);     
			}
			else if (_branches[i].order == 7)
				_branches.emplace_back(&_branches[i], 45);
		}
	}

	auto begin() { return _branches.begin(); }
	auto end() {return _branches.end(); }

private:
	std::vector<Branch> _branches;
};

int main()
{
	sf::RenderWindow window({1900, 1000}, "SFML", sf::Style::Default, sf::ContextSettings{0, 0, 4});
	sf::VertexArray background{sf::Quads, 4};
	background[0].position = {0, 0};
	background[1].position = {1900, 0};
	background[2].position = {1900, 1000};
	background[3].position = {0, 1000};

	background[0].color = background[1].color = {235, 129, 61};
	background[2].color = background[3].color = {235, 63, 103};

	std::vector<Tree> trees;

	for (float i{0}; i < 2; ++i)
		for (float j{0}; j < 4; ++j)
			trees.push_back({{450.f * j + 250.f, 400.f * (i + 1)}});

	sf::Clock clock;
	float lag{0.f};
	const float delta_time{0.0069f};
	while (true)
	{
		sf::Time elapsed = clock.restart();
		lag += elapsed.asSeconds();

		while (lag >= delta_time)
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
					return 0;
			}

			for (auto& tree : trees)
				for (auto& branch : tree)
					branch.update(delta_time);

			lag -= delta_time;	
		}


		window.clear(sf::Color::White);
		window.draw(background);
		for (auto& tree : trees)
			for (const auto& branch : tree)
				window.draw(branch);
		window.display();
	}
}
