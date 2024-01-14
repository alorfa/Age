#include "RotateComp.hpp"
#include <Age/Object/Node.hpp>
#include <Age/Math/Math.hpp>

namespace a_game
{
	void RotateComp::init(const vec3& center, const vec3& size, float speed)
	{
		_center = center;
		_size = size;
		_speed = speed;
	}

	void RotateComp::update(float delta)
	{
		if (_time > Math::TAU)
			_time -= Math::TAU;

		_time += delta * _speed;
		float x = Math::sin(_time) * _size.x;
		float y = Math::cos(_time) * _size.y;
		_node->setPosition(vec3(x, y, 0) + _center);
	}
}

