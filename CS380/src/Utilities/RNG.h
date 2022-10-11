#pragma once
#include "Composition/Singleton.h"

#include <limits>

struct Vector2D;
struct Vector3D;
class RandomNumberGenerator
{
	Singleton(RandomNumberGenerator)

public:
	void Initialize();

	int GetInt(const int min = 0, const int max = INT_MAX);
	bool GetBool();
	float GetFloat(const float min = 0.0f, const float max = FLT_MAX);
	BaleaEngine::Math::Vector2D GetVector2D(const BaleaEngine::Math::Vector2D& min, 
		const BaleaEngine::Math::Vector2D& max);
	BaleaEngine::Math::Vector3D GetVector3D(const BaleaEngine::Math::Vector3D& min, 
		const BaleaEngine::Math::Vector3D& max);
	BaleaEngine::Graphics::Color GetColor(const BaleaEngine::Graphics::Color& min, const BaleaEngine::Graphics::Color& max);
};

#define RNG (&RandomNumberGenerator::Instance())
