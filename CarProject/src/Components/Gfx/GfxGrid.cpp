#include <Shared.h>

GfxGrid::GfxGrid(unsigned width, unsigned height) : mGridSize(width, height)
{
	mShader = ResourceMgr->Load<Graphics::ShaderProgram>("data/Shaders/Water.shader");
	Construct();
	GfxMgr->mEnvMaps.push_back(this);
}

void GfxGrid::Construct()
{
	float stX = 1.0f / mGridSize.x;
	float stY = 1.0f / mGridSize.y;

	Math::Vector2D currCoord;

	// Vertices
	for (unsigned y = 0; y <= mGridSize.y; y++, currCoord.y += stY)
	{
		currCoord.x = 0.0f;

		for (unsigned x = 0; x <= mGridSize.x; x++, currCoord.x += stX)
		{
			Math::Vector3D pos(2.0f * (currCoord.x - 0.5f), 0.0f, 2.0f * (currCoord.y - 0.5f));
			mFaces.push_back(Graphics::Vertex(pos, currCoord, Math::Vector3D(0.0f, 1.0f, 0.0f)));
		}
	}

	// Indices
	for (unsigned y = 0; y < mGridSize.y; y++)
	{
		for (unsigned x = 0; x < mGridSize.x; x++)
		{
			unsigned positions[4] = { y * (mGridSize.x + 1) + x,
									  y * (mGridSize.x + 1) + (x + 1),
									 (y + 1) * (mGridSize.x + 1) + x,
									 (y + 1) * (mGridSize.x + 1) + (x + 1) };

			mIndices.push_back(positions[0]); mIndices.push_back(positions[2]); mIndices.push_back(positions[3]);
			mIndices.push_back(positions[0]); mIndices.push_back(positions[3]); mIndices.push_back(positions[1]);
		}
	}

	// Create and bind VAO
	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	// Create Buffers
	glGenBuffers(1, &mVBO);

	// Bind a buffer of vertices
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);

	glGenBuffers(1, &mEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), &mIndices[0], GL_STATIC_DRAW);

	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, mFaces.size() * sizeof(mFaces[0]), &mFaces[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(mFaces[0]), reinterpret_cast<void*>(offsetof(Graphics::Vertex, mPos)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(mFaces[0]), reinterpret_cast<void*>(offsetof(Graphics::Vertex, mUv)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(mFaces[0]), reinterpret_cast<void*>(offsetof(Graphics::Vertex, mNormal)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(mFaces[0]), reinterpret_cast<void*>(offsetof(Graphics::Vertex, mTangent)));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(mFaces[0]), reinterpret_cast<void*>(offsetof(Graphics::Vertex, mBitangent)));
}

void GfxGrid::OnGui()
{
	/*if (BeginCompOnGui("Grid"))
	{
		// @TODO: This does not work
		if (SetProperty("Grid Size", mGridSize))
			Construct();
		SetProperty("Grid Type", &mGridType, "Terrain Grid\0Noise Grid\0\0");

		ImGui::TreePop();
	}*/
}

void GfxGrid::FromJson(const json & val) noexcept
{
	json tVal = val["Grid"];
}

void GfxGrid::ToJson(json& val) const noexcept
{
	json tVal;
	val["Grid"] = tVal;
}

void GfxGrid::UpdateGrid() // Temp
{
	mEnvMap.UpdateMap(glm::vec3(GetOwner()->GetPosition()));
}
