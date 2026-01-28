#include <GLAD/glad.h>
#include <cglm/cglm.h>

#include "renderer.h"
#include "mesh.h"
#include "entity.h"

void drawMesh(mesh* shape) {
    glBindVertexArray(shape->VAO);

    if (shape->EBO)
        glDrawElements(GL_TRIANGLES, shape->indexCount, GL_UNSIGNED_INT, 0);
    else
        glDrawArrays(GL_TRIANGLES, 0, shape->vertexCount);

    glBindVertexArray(0);
}

void drawEntity(entity* e, unsigned int shaderProgram) {
    glUseProgram(shaderProgram);

    mat4 model;
    transform_build_model(model, &e->transform);

    glUniformMatrix4fv(
        glGetUniformLocation(shaderProgram, "model"),
        1,
        GL_FALSE,
        (float*)model
    );

    drawMesh(e->mesh);
}
