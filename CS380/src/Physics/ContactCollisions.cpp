#include <Shared.h>

bool StaticCircleToStaticCircle(BaleaEngine::Math::Vector2D* Center1, float Radius1, BaleaEngine::Math::Vector2D* Center2, 
	float Radius2, Contact* pResult) {

	//return true if the distance to their centers is less than the sum
	//of their radius
	if (Center1->DistanceSquare(*Center2) <= pow(Radius1 + Radius2, 2)) {

		//if the pResult Exists
		if (pResult) {

			//get the vector that unifies both circles
			BaleaEngine::Math::Vector2D _nm = (*Center2 - *Center1);

			//and get the length of said vector
			float  _len = _nm.Length();

			//calculate the normal of the collision
			pResult->mNormal = _nm / _len;

			//calculat the penetration of the collision
			pResult->mPenetration = Radius1 + Radius2 - _len;

			//calculate the point of intersection of the collision
			pResult->mPi = *Center1 + pResult->mNormal * Radius1;

		}

		//return collided
		return true;

	}

	//return not collided
	return false;

}