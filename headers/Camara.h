#ifndef CAMARA_H
#define CAMARA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "Shader.h"

class Camara {
public:
    // Guarda los vectores de la camara
    glm::vec3 Position;
    glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 camaraMatrix = glm::mat4(1.0f);

    // Previene la camara empiece a saltar al primer click
    bool firstClick = true;

    // Variables de la ventana
    int width;
    int height;

    // Ajusta la velocidad de la camara y su sensibilidad
    float speed = 0.1f;
    float sensitivity = 100.0f;

    // Constructor
    Camara(int width, int height, glm::vec3 position) {
        this->width = width;
        this->height = height;
        Position = position;
    }
    // Actualiza la matriz de la camara al Vertex Shader
    void updateMatrix(float FOVdeg, float nearPlane, float farPlane) {
        // Inicializa las matrices
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        // Hace que la camara mire en la direccion correcta desde la posicion correcta
        view = glm::lookAt(Position, Position + Orientation, Up);
        // Añade perspectiva a la escena
        projection = glm::perspective(glm::radians(FOVdeg), (float)(width/height), nearPlane, farPlane);

        // Indica la nueva matriz camara
        camaraMatrix = projection * view;
    }
    // Exporta la matriz camara a un shader 
    void Matrix(Shader &shader, const char* uniform) {
        // Exporta la matriz de camara al Vertex Shader
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(camaraMatrix));    
    }
    // Inputs de camara
    void Inputs(GLFWwindow* window) {
        // Movimiento de camara con WASD
        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            Position += speed * Orientation;
        }
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            Position += speed * -glm::normalize(glm::cross(Orientation, Up));
        }
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            Position += speed * -Orientation;
        }
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            Position += speed * glm::normalize(glm::cross(Orientation, Up));
        }
        if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            Position += speed * Up;
        }
        if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
            Position += speed * -Up;
        }
        if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            speed = 0.4f;
        } else if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
            speed = 0.1f;
        }

        // Inputs del mouse
        if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            // Esconde el cursor
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

            // Previene que la camara empiece a saltar al primer click
            if(firstClick) {
                glfwSetCursorPos(window, width/2, height/2);
                firstClick = false;
            }

            // Coordenadas del cursor
            double xpos, ypos;
            // Obtiene las coordenadas del cursor
            glfwGetCursorPos(window, &xpos, &ypos);

            // Normaliza y mueve las coordenadas del cursor al centro de la ventana
            // y lo transforma en un angulo
            float rotX = sensitivity*(float)(ypos-(height/2))/height;
            float rotY = sensitivity*(float)(xpos-(width/2))/width;

            // Calcula el cambio vertical en la orientacion
            glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

            // Decide si la nueva orientacion vertical es legal
            // if(abs(glm::angle(newOrientation, Up)-glm::radians(90.0f)) <= glm::radians(85.0f))
            if(!((glm::angle(newOrientation, Up) <= glm::radians(5.0f)) || (glm::angle(newOrientation, -Up) <= glm::radians(5.0f)))) {
                Orientation = newOrientation;
            }

            // Rota la orientacion izquierda y derecha
            Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

            // Actualiza la posicion de la camara al centro para que no se mueva
            glfwSetCursorPos(window, width/2, height/2);

        } else if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            // Deja de esconder el cursor
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            // Previene que la siguiente vez que la camara mire alrededor no salte
            firstClick = true;
        }
    }
};

#endif