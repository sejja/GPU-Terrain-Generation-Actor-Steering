#pragma once

bool StaticPointToStaticCircle(BaleaEngine::Math::Vector2D* P, BaleaEngine::Math::Vector2D* Center, float Radius);
bool StaticPointToStaticRect(BaleaEngine::Math::Vector2D* Pos, BaleaEngine::Math::Vector2D* Rect, float Width, float Height);
bool StaticPointToOrientedRect(BaleaEngine::Math::Vector2D* Pos, BaleaEngine::Math::Vector2D* Rect, float Width, float Height, float AngleRad);
bool StaticCircleToStaticCircle(BaleaEngine::Math::Vector2D* Center0, float Radius0, BaleaEngine::Math::Vector2D* Center1, float Radius1);
bool StaticRectToStaticRect(BaleaEngine::Math::Vector2D* Rect0, float Width0, float Height0, BaleaEngine::Math::Vector2D* Rect1, float Width1, float Height1);
bool StaticRectToStaticCirlce(BaleaEngine::Math::Vector2D* Rect, float Width, float Height, BaleaEngine::Math::Vector2D* Center, float Radius);
bool OrientedRectToStaticCirlce(BaleaEngine::Math::Vector2D* Rect, float Width, float Height, float AngleRad, BaleaEngine::Math::Vector2D* Center, float Radius);