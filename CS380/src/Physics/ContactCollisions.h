#ifndef CONTACT_COLLISION_H
#define CONTACT_COLLISION_H

struct Contact {

	BaleaEngine::Math::Vector2D mPi;
	BaleaEngine::Math::Vector2D mNormal;
	float	mPenetration;

};

bool StaticCircleToStaticCircle(BaleaEngine::Math::Vector2D* Center1, float Radius1, BaleaEngine::Math::Vector2D* Center2, float Radius2, Contact* pResult);


#endif 