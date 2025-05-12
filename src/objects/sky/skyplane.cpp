#include "skyplane.h"

#include "../../../helper/noisetex.h"
#include <glm/ext/matrix_transform.hpp>

using glm::mat4;
using glm::vec3;


SkyPlane::~SkyPlane()
{
    defualt_cleanup();

    if (handle[0] != 0) glDeleteBuffers(1, &handle[0]);
    if (handle[1] != 0) glDeleteBuffers(1, &handle[1]);

    if (quad != 0) glDeleteVertexArrays(1, &quad);

    if (noiseTex != 0) glDeleteTextures(1, &noiseTex);

    handle[0] = handle[1] = 0;
    quad = 0;
    noiseTex = 0;
}

void SkyPlane::init() {

    GLfloat verts[] = {
        -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f
    };
    GLfloat tc[] = {
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
    };

    // creates buffers
    glGenBuffers(2, handle);

    // vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
    glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(float), verts, GL_STATIC_DRAW);

    // texture coordinate buffer
    glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), tc, GL_STATIC_DRAW);

    // creates VAO
    glGenVertexArrays(1, &quad);
    glBindVertexArray(quad);

    glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    noiseTex = NoiseTex::generate2DTex(40.0f, .9f, 1024, 1024, true);

    modelMatrix_ = mat4(1.0f);
    modelMatrix_ = glm::translate(modelMatrix_, vec3(0.0f, 300.f, 0.0f));
    modelMatrix_ = glm::scale(modelMatrix_, vec3(1000));
    modelMatrix_ = glm::rotate(modelMatrix_, glm::radians(90.f), vec3(1.0f, 0.0f, 0.0f));
}

void SkyPlane::update(float t)
{
}

void SkyPlane::render(const glm::mat4& view, const glm::mat4& projection, GLSLProgram& prog)
{
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    prog.setUniform("NoiseTex", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, noiseTex);

    glm::mat4 mv = view * modelMatrix_;

    prog.setUniform("MVP", projection * mv);

    glBindVertexArray(quad);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDepthMask(GL_TRUE);

    glBindVertexArray(0);

    glDisable(GL_BLEND);
}

void SkyPlane::renderDepth(GLSLProgram& depthProg)
{
}


