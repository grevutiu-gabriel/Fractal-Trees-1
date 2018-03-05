#pragma once

#include <ostream>

inline std::ostream& operator<<(std::ostream& os_, const sf::Vector2f& vec_)
{
	os_ << '[' << vec_.x << ", " << vec_.y << ']';
	return os_;
}


struct FloatPair
{
	float first, second;
};

inline bool inBounds(float value_, FloatPair range_)
{
	return value_ >= range_.first && value_ <= range_.second; 
}

inline float map(float value_, FloatPair original_range_, FloatPair new_range_)
{
	return (value_- original_range_.first) / (original_range_.second - original_range_.first)
		* (new_range_.second - new_range_.first) + new_range_.first;
}
