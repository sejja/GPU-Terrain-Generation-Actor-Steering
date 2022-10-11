//
//	Vertex.h
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright © 2021 Digipen. All Rights reserved
//

#ifndef _VERTEX__H_
#define _VERTEX__H_

namespace BaleaEngine {
	namespace Graphics {
		struct Vertex
		{
			Vertex() = default;
			Vertex(const Math::Vector3D& pos, const Math::Vector2D& uv, const Math::Vector3D& normal) :
				mPos(pos), mUv(uv), mNormal(normal) {}

			Math::Vector3D mPos;
			Math::Vector2D mUv;
			Math::Vector3D mNormal;
			Math::Vector3D mTangent;
			Math::Vector3D mBitangent;
		};
	}
}

#endif