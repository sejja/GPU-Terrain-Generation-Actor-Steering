#include <Shared.h>


void debug_draw_segment(const glm::vec3& s, const glm::vec3& e, const glm::vec4& color)
{
    // Generate and set up VAO and VBO
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glm::vec3 vertices[2] = { s, e };
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    auto& cam = Editor->isPlaying() ? *Editor->GetCurrentScene()->mViewCamera : *Editor->GetEditorCamera();

    glm::mat4 projView = cam.getProjMtx() * cam.getViewMtx();

    BaleaEngine::Graphics::ShaderProgram* shader = ResourceMgr->Load<BaleaEngine::Graphics::ShaderProgram>("data/Shaders/color.shader")->get();

    // Draw segment
    shader->Bind();
    glUniformMatrix4fv(0, 1, GL_FALSE, &projView[0][0]);
    glUniform4fv(1, 1, &color[0]);
    glDrawArrays(GL_LINES, 0, 2);

    // Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Free Gfx memory
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}