#include <Shared.h>

void InstancedRenderable::InitializeBuffers()
{
	if (mPosList.empty())
		return;

	glGenBuffers(1, &mPosVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mPosVBO);
	glBufferData(GL_ARRAY_BUFFER, mPosList.size() * sizeof(glm::vec3), &mPosList[0], GL_DYNAMIC_DRAW);
	glGenBuffers(1, &mTexIdxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mTexIdxVBO);
	glBufferData(GL_ARRAY_BUFFER, mOffsetList.size() * sizeof(unsigned int), &mOffsetList[0], GL_DYNAMIC_DRAW);

	for (unsigned i = 0; i < mModel->GetMeshCount(); i++)
	{
		glBindVertexArray(mModel->GetMesh(i)->GetHandleVAO());

		// Color data
		glBindBuffer(GL_ARRAY_BUFFER, mTexIdxVBO);

		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(unsigned int), (void*)0);

		// Model matrix
		glBindBuffer(GL_ARRAY_BUFFER, mPosVBO);

		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}

	UpdateAttributeData();
}

void InstancedRenderable::UpdateAttributeData()
{
	glBindBuffer(GL_ARRAY_BUFFER, mPosVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, mPosList.size() * sizeof(glm::vec3), &mPosList[0]);

	glBindBuffer(GL_ARRAY_BUFFER, mTexIdxVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, mOffsetList.size() * sizeof(unsigned int), &mOffsetList[0]);
}

InstancedRenderable::InstancedRenderable()
{
	stl::PageAllocator<BaleaEngine::Graphics::Model> alloc;

	SetShader("data/Shaders/Default_Instanced.shader");
	mModel = alloc.allocate();
	alloc.construct(mModel, "data/Model/Default Shapes/Cube.obj");
	mCubemap.push_back(ResourceMgr->Load<BaleaEngine::Graphics::Cubemap>("data/Skybox/MinecraftStone.skybox"));
	mCubemap.push_back(ResourceMgr->Load<BaleaEngine::Graphics::Cubemap>("data/Skybox/MinecraftSand.skybox"));
	mCubemap.push_back(ResourceMgr->Load<BaleaEngine::Graphics::Cubemap>("data/Skybox/MinecraftDirt.skybox"));
	mCubemap.push_back(ResourceMgr->Load<BaleaEngine::Graphics::Cubemap>("data/Skybox/MinecraftSnow.skybox"));
}

InstancedRenderable::~InstancedRenderable() {
	stl::PageAllocator<BaleaEngine::Graphics::Model> alloc;
	alloc.destroy(mModel);
	alloc.deallocate(mModel);
}

void InstancedRenderable::AddEntity(const glm::vec3& pos, const unsigned int texOffset)
{
	mPosList.push_back(pos);
	mOffsetList.push_back(texOffset);
}

void InstancedRenderable::Clear() {
	mPosList.clear();
	mOffsetList.clear();
}

void InstancedRenderable::Draw()
{
	BaleaEngine::Graphics::ShaderProgram* shader = GetShader()->get();

	// Skybox
	for (unsigned i = 0; i < mCubemap.size(); i++)
	{
		BaleaEngine::Graphics::Cubemap* skybox = mCubemap[i]->get();
		shader->SetUniform(("uSkybox_" + std::to_string(i)).c_str(), i);
		glActiveTexture(GL_TEXTURE0 + i);
		skybox->Bind();
	}
	glActiveTexture(GL_TEXTURE0);

	for (unsigned int i = 0; i < mModel->GetMeshCount(); i++)
	{
		// Draw mesh
		glBindVertexArray(mModel->GetMesh(i)->GetHandleVAO());
		glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(mModel->GetMesh(i)->GetNumberOfIndices()), GL_UNSIGNED_INT,
			0, mPosList.size());
		// Set everything back to default
		//glBindVertexArray(0);
	}
}