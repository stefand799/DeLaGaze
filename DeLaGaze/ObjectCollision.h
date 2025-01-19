#pragma once
#include "Object.h"

class ObjectCollision {
public:
	ObjectCollision(std::shared_ptr<Object> obj1, std::shared_ptr<Object> obj2, float collisionTime);
	ObjectCollision(std::tuple<std::shared_ptr<Object>, std::shared_ptr<Object>, float>&& values);
	ObjectCollision(std::shared_ptr<Object> obj, float collisionTime);
	ObjectCollision(const ObjectCollision&) = default;
	ObjectCollision(ObjectCollision&&) = default;
	ObjectCollision& operator=(const ObjectCollision&) = default;
	ObjectCollision& operator=(ObjectCollision&&) = default;

	bool operator<(const ObjectCollision& other) const;


	std::shared_ptr<Object> first;
	std::shared_ptr<Object> second;
	float time;
	bool isBorderCollision;
};