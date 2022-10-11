#include <Shared.h>

void RandomNumberGenerator::Initialize()
{
	srand((unsigned)time(0));
}

int RandomNumberGenerator::GetInt(const int min, const int max)
{
	return min + std::rand() % (max - min);
}

bool RandomNumberGenerator::GetBool()
{
	return static_cast<bool>(std::rand() % 2);
}

float RandomNumberGenerator::GetFloat(const float min, const float max)
{
	return min + static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX / (max - min));
}

BaleaEngine::Math::Vector2D RandomNumberGenerator::GetVector2D(const BaleaEngine::Math::Vector2D& min, const 
	BaleaEngine::Math::Vector2D& max)
{
	BaleaEngine::Math::Vector2D result;
	result.x = GetFloat(min.x, max.x);
	result.y = GetFloat(min.y, max.y);
	return result;
}

BaleaEngine::Math::Vector3D RandomNumberGenerator::GetVector3D(const BaleaEngine::Math::Vector3D& min,
	const BaleaEngine::Math::Vector3D& max)
{
	BaleaEngine::Math::Vector3D result;
	result.x = GetFloat(min.x, max.x);
	result.y = GetFloat(min.y, max.y);
	result.z = GetFloat(min.z, max.z);
	return result;
}

BaleaEngine::Graphics::Color RandomNumberGenerator::GetColor(const BaleaEngine::Graphics::Color& min, const
	BaleaEngine::Graphics::Color& max)
{
	BaleaEngine::Graphics::Color result;
	result.r = GetFloat(min.r, max.r);
	result.g = GetFloat(min.g, max.g);
	result.b = GetFloat(min.b, max.b);
	result.a = GetFloat(min.a, max.a);
	return result;
}