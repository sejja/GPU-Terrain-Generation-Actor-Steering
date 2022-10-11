#include <Shared.h>

void Water::Draw()
{
	if (!mShader)
		return;

	Graphics::ShaderProgram* pShader = mShader->get();
	pShader->Bind();

	glActiveTexture(GL_TEXTURE0);
	GfxMgr->mNoiseTex["Worley"]->BindTexture();
	pShader->SetUniform("uWorleyNoise", 0);
	pShader->SetUniform("uWaveIncrement", 0.1f);
	pShader->SetUniform("uBaseColor", Math::Vector3D(0.3f, 0.5f, 1.0f));
	pShader->SetUniform("uWaveColor", Math::Vector3D(0.5f, 1.0f, 1.0f));


	glActiveTexture(GL_TEXTURE1);
	Graphics::Cubemap* skybox = GfxMgr->mSkybox->get();
	skybox->Bind();
	pShader->SetUniform("uEnvMap", 1);


	/*pShader->SetUniform("uEnvMap", 1);
	glActiveTexture(GL_TEXTURE1);
	mEnvMap.Bind();*/

	glBindVertexArray(mVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);

	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndices.size()), GL_UNSIGNED_INT, 0);
}
