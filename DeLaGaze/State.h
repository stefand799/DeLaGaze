#pragma once

#include <cstdint>

enum class State : uint8_t {
	Idle,
	Moving,
	Shooting,
};