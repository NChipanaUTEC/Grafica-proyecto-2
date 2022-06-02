#ifndef MESH_H
#define MESH_H

#include <string>

#include "VAO.h"
#include "EBO.h"
#include "Camara.h"
#include "Textura.h"

class Mesh {
public:
    std::vector<Vertice> vertices;
    std::vector<GLuint> indices;
    std::vector<Textura> texturas;
    // Guardar el VAO en public para poder usarlo en draw
    VAO vao;

    // Inicializa el mesh
    Mesh(std::vector<Vertice>& vertices, std::vector<GLuint>& indices, std::vector<Textura>& texturas) {
        this->vertices = vertices;
        this->indices = indices;
        this->texturas = texturas;

        vao.Bind();
        // Generar Vertex Buffer Object y lo enlaza a los vertices
        VBO VBO(vertices);
        // Generar Element Buffer Object y lo enlaza a los indices
        EBO EBO(indices);
        // Enlaza atributos VBO como coordenadas y colores a vao
        vao.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertice), (void*)0);
        vao.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertice), (void*)(3*sizeof(float)));
        vao.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertice), (void*)(6*sizeof(float)));
        vao.LinkAttrib(VBO, 3, 2, GL_FLOAT, sizeof(Vertice), (void*)(9*sizeof(float)));
        // Desenlazar todos los objetos para evitar modificarlos en el resto del programa
        vao.Unbind();
        VBO.Unbind();
        EBO.Unbind();
    }

    // Dibuja el mesh
    void Draw(Shader& shader, Camara& camara) {
        // Enlazar el shader para poder acceder a los uniforms
        shader.Activate();
        vao.Bind();

        // Saber la cantidad de cada tipo de textura hay
        unsigned int numDifuso = 0;
        unsigned int numSpecular = 0;

        for(unsigned int i = 0; i < texturas.size(); i++) {
            std::string num;
            std::string tipo = texturas[i].tipo;
            if(tipo == "difuso") {
                num = std::to_string(numDifuso++);
            } else if(tipo == "specular") {
                num = std::to_string(numSpecular++);
            }
            texturas[i].texUnit(shader,(tipo+num).c_str(),i);
            texturas[i].Bind();
        }

        // Hacerse cargo de la matriz camara
        glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camara.Position.x, camara.Position.y, camara.Position.z);
        camara.Matrix(shader, "camMatrix");

        // Dibujar el mesh
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    }
};

#endif