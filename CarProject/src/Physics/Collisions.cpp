/*!
*	\file		Collisions.cpp
*	\brief		Logic for the basic 2D collisions
*	\details	Contains the implementations for Basic Collisions
*	\author		Diego Revilla - diego.r@.edu
*	\date		19/02/2020
*	\copyright	Copyright  Institute Of Technology. All Rights Reserved
*
*/

#include <Shared.h>

//! ---------------------------------------------------------------------------
// \fn		StaticPointToStaticCircle
// \brief	This function determines if a static point is inside a static circle
// 			- P:		The point to test for circle containment
// 			- Center:	Center of the circle
// 			- Radius:	Radius of the circle
// 
//  \return	true if the point is contained in the circle, false otherwise.
// ---------------------------------------------------------------------------
bool StaticPointToStaticCircle(BaleaEngine::Math::Vector2D* P, BaleaEngine::Math::Vector2D* Center, float Radius) {

	//return true if the distance to their centers is less than the sum
	//of their radious
	return (P->DistanceSquare(*Center)) <= pow(Radius, 2) ? true : false;

}