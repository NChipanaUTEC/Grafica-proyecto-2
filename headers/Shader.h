#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

// Lee el contenido de un archivo de texto y lo devuelve como un string
std::string get_file_contents(const char *filename) {
    std::ifstream in(filename, std::ios::binary);
    if(in) {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return(contents);
    }
    throw(errno);
}

class Shader {
public:
    // ID de referencia del Shader Program
    GLuint ID;
    // Constructor para crear el Shader Program
    Shader(const char* vertexFile, const char* fragmentFile) {
        // Leer el contenido de los archivos de texto
        std::string vertexCode = get_file_contents(vertexFile);
        std::string fragmentCode = get_file_contents(fragmentFile);

        // Convertir los strings a char arrays
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        // Crear un objeto Vertex Shader y obtener su referencia 
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        // Enviar el codigo Vertex Sahder al objeto Vertex Shader
        glShaderSource(vertexShader, 1, &vShaderCode, NULL);
        // Compilar el Vertex Shader a codigo maquina
        glCompileShader(vertexShader);
        // Verificar que el Vertex Shader se compilo correctamente
        compileErrors(vertexShader, "VERTEX");

        // Crear un objeto Fragment Shader y obtener su referencia
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        // Enviar el codigo Fragment Shader al objeto Fragment Shader
        glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
        // Compilar el Fragment Shader a codigo maquina
        glCompileShader(fragmentShader);
        // Verificar que el Fragment Shader se compilo correctamente
        compileErrors(fragmentShader, "FRAGMENT");

        // Crear el objeto Shader Program y obtener su referencia
        ID = glCreateProgram();
        // Enlazar el Vertex y Fragment Shader al Shader Program
        glAttachShader(ID, vertexShader);
        glAttachShader(ID, fragmentShader);
        // Enlazar todos los shaders al Shader Program
        glLinkProgram(ID);
        // Verificar si las shaders se enlazaron correctamente
        compileErrors(ID, "PROGRAM");

        // Borrar los shaders ya no se van a usar
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    // Activa el Shader Program
    void Activate() {
        glUseProgram(ID);
    }
    // Elimina el Shader Program
    void Delete() {
        glDeleteProgram(ID);
    }
private:
    // Verifica si se compilo correctamente las shaders
    void compileErrors(unsigned int shader, const char* type) {
        GLint hasCompiled;
        char infoLog[1024];
        if(type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
            if(hasCompiled == GL_FALSE) {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- \n";
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
            if(hasCompiled == GL_FALSE) {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- \n";
            }
        }
    }
};

#endif