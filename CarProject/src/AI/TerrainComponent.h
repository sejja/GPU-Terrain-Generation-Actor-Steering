#pragma once

namespace CarProj {
	class Terrain : public BaleaEngine::Graphics::RenderComp {
	public:
		Terrain();
		void FromJson(const json& val) noexcept;
		void ToJson(json& val) const noexcept;
		void Draw() noexcept override;
		void Update() noexcept override;
		void OnGui() noexcept override;
	};
}