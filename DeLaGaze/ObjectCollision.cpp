#include "ObjectCollision.h"


ObjectCollision::ObjectCollision(std::shared_ptr<Object> obj1, std::shared_ptr<Object> obj2, float collisionTime) :
	first{ obj1 },
	second{ obj2 },
	time{ collisionTime },
	isBorderCollision{ false }
{}

ObjectCollision::ObjectCollision(std::tuple<std::shared_ptr<Object>, std::shared_ptr<Object>, float>&& values) :
	first{ std::get<0>(values) },
	second{ std::get<1>(values) },
	time{ std::get<2>(values) },
	isBorderCollision{ false }
{}

ObjectCollision::ObjectCollision(std::shared_ptr<Object> obj, float collisionTime) :
	first{ obj },
	second{ nullptr },
	time{ collisionTime },
	isBorderCollision{ true }
{}

bool ObjectCollision::operator<(const ObjectCollision& other) const {
	return this->time < other.time;
}