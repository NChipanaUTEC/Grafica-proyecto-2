#include "../headers/Mesh.h"

using namespace std;

const unsigned int width = 800;
const unsigned int height = 600;

Vertice vertices[] = {

};

GLuint indices[] = {

};

Vertice lightVertices[] = {

};

GLuint lightIndices[] = {

};

int main()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // Crear la ventana
    GLFWwindow* window = glfwCreateWindow(width, height, "Proyecto Grafica", NULL, NULL);
    if(window==NULL) {
        cout << "No se pudo crear la ventana" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    gladLoadGL();

    glViewport(0, 0, width, height);

    Textura texturas[] = {

    };

    Shader lightShader("../shaders/light.vs", "../shaders/light.fs");
    vector<Vertice> lightV(lightVertices,lightVertices+sizeof(lightVertices)/sizeof(Vertice));
    vector<GLuint> lightI(lightIndices,lightIndices+sizeof(lightIndices)/sizeof(GLuint));
    vector<Textura> lightT(texturas,texturas+sizeof(texturas)/sizeof(Textura));
    Mesh light(lightV,lightI,lightT);

    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, lightPos);

    lightShader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
    glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);

    glEnable(GL_DEPTH_TEST);

    Camara camara(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.0f,0.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camara.Inputs(window);
        camara.updateMatrix(45.0f,0.1f,100.0f);

        light.Draw(lightShader,camara);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    lightShader.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}