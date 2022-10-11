#pragma once

namespace CarProj
{
	struct Project;

	struct Grid
	{
		Grid() : mTiles(nullptr) {}
		Grid(const unsigned size_x, const unsigned size_y, const unsigned size_z);

		~Grid();
		DONTDISCARD bool& GetTile(const glm::ivec3& position) const noexcept;
		void Allocate();
		void UpdateMap();
		void Clean();
		void UploadToGPU();

	public:
		bool* mTiles = nullptr;
		glm::uvec3 size;
		BaleaEngine::GameObject* pObj = nullptr;

		float mObjScale = 1.0f;

		BaleaEngine::Graphics::PerlinNoise mNoise;
	};
}