//
//	Collider.h
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright © 2021 Digipen. All Rights reserved
//

#ifndef _COLLIDER__H_
#define _COLLIDER__H_

namespace BaleaEngine {
	namespace Physics {
		struct Collider : public Component
		{
#pragma region // Definitions
			enum class eColliderType : int
			{
				e_Box,
				e_Sphere
			};
#pragma endregion

#pragma region // Constructors & Destructors
			Collider() noexcept;
			Collider(eColliderType colliderType) noexcept;
			~Collider() noexcept;
#pragma endregion

#pragma region // Functions
			void Initialize() noexcept override;
			void Update() noexcept override;
			void OnGui() noexcept override;
			void FromJson(const json& val) noexcept override;
			void ToJson(json& val) const noexcept override;
			float mRadious = 0.3;
#pragma endregion

#pragma region // Members
		private:
			eColliderType mColliderType;
#pragma endregion
		};
	}
}

#endif