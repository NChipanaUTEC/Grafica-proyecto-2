#ifndef VAO_H
#define VAO_H

#include <glad/glad.h>
#include "VBO.h"

class VAO {
public:
    GLuint ID;
    VAO() {
        glGenVertexArrays(1, &ID);
    }
    // Enlaza el atributo VBO al VAO usando el layout
    void LinkAttrib(VBO &VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) {
        VBO.Bind();
        glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
        glEnableVertexAttribArray(layout);
        VBO.Unbind();
    }
    void Bind() {
        glBindVertexArray(ID);
    }
    void Unbind() {
        glBindVertexArray(0);
    }
    void Delete() {
        glDeleteVertexArrays(1, &ID);
    }
};

#endif