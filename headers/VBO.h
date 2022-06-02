#ifndef VBO_H
#define VBO_H

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <vector>

struct Vertice {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
    glm::vec2 texCoords;
};

class VBO {
public:
    // ID de referencia del Vertex Buffer Object
    GLuint ID;
    // Constructor para crear el Vertex Buffer Object y enlazar sus vertices
    VBO(std::vector<Vertice>& vertices) {
        glGenBuffers(1, &ID);
        glBindBuffer(GL_ARRAY_BUFFER, ID);
        glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertice), vertices.data(), GL_STATIC_DRAW);
    }

    // Enlaza el VBO
    void Bind() {
        glBindBuffer(GL_ARRAY_BUFFER, ID);
    }
    // Desenlaza el VBO
    void Unbind() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    // Destructor para eliminar el VBO
    void Delete() {
        glDeleteBuffers(1, &ID);
    }
};

#endif