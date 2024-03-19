#include "Ball.hpp"
#include "Age/Object/Node.hpp"
#include "Age/Math/Math.hpp"
#include "Age/Components/MeshComponent.hpp"
#include "Age/egd.hpp"

namespace a_game
{
	Ball::Ball(Node& node)
	{
		this->node = &node;
		node.setScale(0.15f);
		MeshComponent::addModel(node, egd.models.load(egd.res / "model/sphere.obj"));
		MeshComponent::setShader(node, egd.shaders.load(egd.res / "shader/ball.asl"));
	}
	Ball& Ball::setVelocity(const vec3& v)
	{
		velocity = v;
		return *this;
	}
	void Ball::handleEvents(const EventHandler& ev, float delta)
	{
	}

	void Ball::update(float delta)
	{
		time += delta;
		if (time > 10.f)
		{
			node->destroy();
			return;
		}
		velocity.z -= 9.8f * delta;
		vec3 newPos = node->getTransform().getPosition() + velocity * delta;
		if (newPos.z < 0.15f)
		{
			newPos.z = 0.15f;
			velocity.z = -velocity.z * 0.7f;
			if (velocity.z < 0.1f)
				velocity.z = 0.f;
			velocity.x *= 0.9f;
			velocity.y *= 0.9f;
		}
		node->setPosition(newPos);
	}
}
