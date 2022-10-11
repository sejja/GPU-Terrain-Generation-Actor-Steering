#pragma once
#include "RenderComp.h"
#include "Graphics/Model/Model.h"

struct InstancedRenderable : public BaleaEngine::Graphics::RenderComp
{
	friend struct BaleaEngine::Graphics::Model;

	InstancedRenderable();
	~InstancedRenderable();

	BaleaEngine::Graphics::Model* mModel;
	stl::Array<BaleaEngine::TResource<BaleaEngine::Graphics::Cubemap>*> mCubemap;
	void AddEntity(const glm::vec3& pos, const unsigned int texOffset);
	void Clear();
	void InitializeBuffers();

	void UpdateAttributeData();

	void Draw() override;

	void OnGui() override {}
	void FromJson(const json& val) {}
	void ToJson(json& val) const {}

protected:

	stl::Array<glm::vec3> mPosList;
	stl::Array<unsigned int> mOffsetList;

private:
	unsigned  mPosVBO;
	unsigned  mTexIdxVBO;
};