//
//	Factory.cpp
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright © 2021 . All Rights reserved
//

#include <Shared.h>

namespace BaleaEngine {
	// ------------------------------------------------------------------------
	/*! Initialize
	*
	*   Adds the Default Components to the Factory Manager
	*   TODO: Might implement an automation with the "Krew Header Tool"
	*   in the future if time allows
	*/ // ---------------------------------------------------------------------
	void FactoryManager::Initialize() noexcept
	{
		// ----------------- COMPONENTS -----------------
		RegisterCreator<BaleaEngine::Physics::RigidBody>("RigidBody");
		RegisterCreator<BaleaEngine::Physics::Collider>("Collider");
		RegisterCreator<BaleaEngine::Graphics::MeshRenderer>("MeshRenderer");
		RegisterCreator<BaleaEngine::Graphics::Light>("Light");
		RegisterCreator<CarProj::Terrain>("Terrain");
		RegisterCreator<CarProj::CarBehavior>("CarBehavior");
		RegisterCreator<BaleaEngine::EditorCamera>("EditorCamera");
		RegisterCreator<ParticleEmitter>("ParticleEmitter");
	}
}