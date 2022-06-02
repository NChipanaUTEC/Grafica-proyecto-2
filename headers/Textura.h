#ifndef TEXTURA_H
#define TEXTURA_H

#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Shader.h"

class Textura {
public:
    GLuint ID;
    const char* tipo;
    GLuint unit;
    Textura(const char* image, const char* texType, GLuint slot, GLenum format, GLenum pixelType) {
        // Asigna el tipo de textura
        tipo = texType;
        
        // Guarda el ancho, alto y numero de color channels de la imagen
        int imgWidth, imgHeight, numColCh;
        // Voltea la imagen asi aparece de arriba a abajo
        stbi_set_flip_vertically_on_load(true);
        // Lee la imagen de un archivo y lo guarda en bytes
        unsigned char* bytes = stbi_load(image, &imgWidth, &imgHeight, &numColCh, 0);

        // Genera el objeto textura de OpenGL
        glGenTextures(1, &ID);
        // Asigna la textura a un Texture Unit
        glActiveTexture(GL_TEXTURE0 + slot);
        unit = slot;
        glBindTexture(GL_TEXTURE_2D, ID);

        // Configura el tipo de algoritmo que se usa para hacer la imagen grande o pequeña
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // Configura la forma en la que la textura se repite (si es que ocurre)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Si se usa GL_CLAMP_TO_BORDER
        // float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
        // glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);

        // Asigna la imagen al objeto textura de OpenGL
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, format, pixelType, bytes);
        // Genera MipMaps
        glGenerateMipmap(GL_TEXTURE_2D);

        // Elimina los datos de la memoria ya que ahora es un objeto textura de OpenGL
        stbi_image_free(bytes);

        // Desasigna la textura para evitar modificaciones
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    // Asigna el Texture Unit a una textura
    void texUnit(Shader &shader, const char* uniform, GLuint unit) {
        // Obtener la locacion del uniform
        GLuint text0Uni = glGetUniformLocation(shader.ID, uniform);
        // Se debe activar el shader antes de cambiar el valor de uniform
        shader.Activate();
        // Asigna el valor del uniform
        glUniform1i(text0Uni, unit);
    }
    // Une la textura
    void Bind() {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, ID);
    }
    // Desune la textura
    void Unbind() {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    // Borra la textura
    void Delete() {
        glDeleteTextures(1, &ID);
    }
};

#endif