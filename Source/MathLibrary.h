#pragma once
#include <cmath>
#include "Engine/EngineTypes.h"


namespace Math {

	// Add one vector to another
	static exVector2 Add(exVector2 v1, exVector2 v2)
	{
		return { v1.x + v2.x, v1.y + v2.y };
	}

	// Subtract one vector from another
	static exVector2 Substract(exVector2 v1, exVector2 v2)
	{
		return { v1.x - v2.x, v1.y - v2.y };
	}

	// Divide vector with a float
	static exVector2 Divide(exVector2 v1, float number)
	{
		return { v1.x / number, v1.y / number };
	}

	// Multiple vector with a float
	static exVector2 Multiply(exVector2 v1, float number)
	{
		return { v1.x * number, v1.y * number };
	}
	
	// Normalize Vector
	static exVector2 Normalize(exVector2 v1) {

		const float magnitude = sqrt(pow(v1.x, 2) + pow(v1.y, 2));
		return { v1.x / magnitude, v1.y / magnitude };
	}

	// Dot product
	static float Dot(exVector2 v1, exVector2 v2) {

		return { v1.x * v2.x + v1.y * v2.y };
	}

	// Distance between two vectors
	static float Distance(exVector2 v1, exVector2 v2) {

		return sqrt(pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2));
	}




};