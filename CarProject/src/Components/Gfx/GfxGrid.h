#pragma once

struct GfxGrid : public BaleaEngine::Graphics::RenderComp
{
	using FaceList = std::vector<BaleaEngine::Graphics::Vertex>;
	using IndexList = std::vector<unsigned>;

	enum mGridType
	{
		eTerrainGrid,
		eNoiseGrid
	};

	GfxGrid(unsigned width = 50, unsigned height = 50);

	void Construct(); // With the current parameters

	void OnGui() override;

	void FromJson(const json& val) noexcept;
	void ToJson(json& val) const noexcept;

	void UpdateGrid(); // Temp

public:
	FaceList mFaces;
	IndexList mIndices;

	BaleaEngine::Graphics::Cubemap mEnvMap;

	glm::uvec2 mGridSize;
	int mGridType = eNoiseGrid;

protected:
	unsigned mVAO, mVBO, mEBO;
};